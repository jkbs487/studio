import fs from 'fs'
import path from 'path'
import { fileURLToPath } from 'url'

const __dirname = path.dirname(fileURLToPath(import.meta.url))
const ROOT_DIR = path.resolve(__dirname, '../..')
const OUT_DIR = path.resolve(__dirname, '../')

const LANGUAGES_DIR = path.join(ROOT_DIR, 'languages')
const EXPERIMENTS_DIR = path.join(ROOT_DIR, 'experiments')

function scanMeta(baseDir) {
  const experiments = []
  if (!fs.existsSync(baseDir)) return experiments

  for (const entry of fs.readdirSync(baseDir, { withFileTypes: true })) {
    if (!entry.isDirectory()) continue
    const dirPath = path.join(baseDir, entry.name)
    const metaPath = path.join(dirPath, 'meta.json')

    if (fs.existsSync(metaPath)) {
      try {
        const meta = JSON.parse(fs.readFileSync(metaPath, 'utf-8'))
        meta._path = path.relative(ROOT_DIR, dirPath)
        meta._dir = dirPath
        // 推断 language：如果缺失，从路径推断
        if (!meta.language) {
          const rel = path.relative(LANGUAGES_DIR, dirPath)
          if (!rel.startsWith('..')) {
            meta.language = rel.split(path.sep)[0] || 'unknown'
          } else {
            const relExp = path.relative(EXPERIMENTS_DIR, dirPath)
            if (!relExp.startsWith('..')) {
              meta.language = 'multi'
            }
          }
        }
        experiments.push(meta)
      } catch {
        console.warn(`Failed to parse ${metaPath}`)
      }
    } else {
      // 递归扫描子目录
      experiments.push(...scanMeta(dirPath))
    }
  }
  return experiments
}

function readCodeFiles(dir) {
  const files = []
  const exts = ['.cpp', '.py', '.go', '.rs', '.h', '.hpp', '.c']
  for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
    if (!entry.isFile()) continue
    const ext = path.extname(entry.name)
    if (exts.includes(ext)) {
      files.push({
        name: entry.name,
        ext,
        content: fs.readFileSync(path.join(dir, entry.name), 'utf-8')
      })
    }
  }
  return files
}

function getLangFromExt(ext) {
  const map = { '.cpp': 'cpp', '.h': 'cpp', '.hpp': 'cpp', '.c': 'c', '.py': 'python', '.go': 'go', '.rs': 'rust' }
  return map[ext] || 'text'
}

function slugify(name) {
  return name.replace(/[^a-zA-Z0-9-]/g, '-').replace(/-+/g, '-').toLowerCase()
}

function generateExperimentPage(meta) {
  const readmePath = path.join(meta._dir, 'README.md')
  let readme = ''
  if (fs.existsSync(readmePath)) {
    readme = fs.readFileSync(readmePath, 'utf-8')
  }

  const codeFiles = readCodeFiles(meta._dir)
  let codeSection = ''
  if (codeFiles.length > 0) {
    codeSection = '\n## 源码\n\n'
    for (const f of codeFiles) {
      codeSection += `<details>\n<summary>${f.name}</summary>\n\n\`\`\`${getLangFromExt(f.ext)}\n${f.content}\n\`\`\`\n\n</details>\n\n`
    }
  }

  const tags = (meta.tags || []).map(t => `\`${t}\``).join(' ')
  const frontMatter = `---
title: ${meta.name}
description: ${meta.description}
---

# ${meta.name}

<div class="experiment-meta">

| 属性 | 值 |
|------|-----|
| 语言 | ${meta.language || 'N/A'} |
| 版本 | ${meta.language_version || 'N/A'} |
| 创建时间 | ${meta.created || 'N/A'} |
| 来源 | ${meta.source || 'N/A'} |
| AI 参与 | ${meta.ai_participation || 'N/A'} |
| 标签 | ${tags || '无'} |

</div>

## 描述

${meta.description}

${readme}
${codeSection}

<style>
.experiment-meta {
  margin: 1rem 0;
}
.experiment-meta table {
  margin: 0;
}
</style>
`
  return frontMatter
}

function main() {
  console.log('Scanning experiments...')

  const langExps = scanMeta(LANGUAGES_DIR)
  const expExps = scanMeta(EXPERIMENTS_DIR)
  const allExps = [...langExps, ...expExps]

  // 确保输出目录并清理旧文件
  const expOutDir = path.join(OUT_DIR, 'experiments')
  const langOutDir = path.join(OUT_DIR, 'languages')
  fs.mkdirSync(expOutDir, { recursive: true })
  fs.mkdirSync(langOutDir, { recursive: true })

  // 清理旧页面（保留 _data.json 等元数据文件由后续生成覆盖）
  for (const f of fs.readdirSync(expOutDir)) {
    if (f.endsWith('.md') && f !== 'index.md') fs.unlinkSync(path.join(expOutDir, f))
  }
  for (const f of fs.readdirSync(langOutDir)) {
    if (f.endsWith('.md')) fs.unlinkSync(path.join(langOutDir, f))
  }

  // 生成每个实验的页面
  const expLinks = []
  for (const meta of allExps) {
    const slug = slugify(meta.name)
    const pageContent = generateExperimentPage(meta)
    const outPath = path.join(expOutDir, `${slug}.md`)
    fs.writeFileSync(outPath, pageContent)
    expLinks.push({
      name: meta.name,
      slug,
      language: meta.language || 'unknown',
      description: meta.description,
      created: meta.created,
      tags: meta.tags || [],
      path: meta._path
    })
  }

  // 统计数据
  const stats = { total: allExps.length, languages: {} }
  for (const e of allExps) {
    const lang = e.language || 'unknown'
    stats.languages[lang] = (stats.languages[lang] || 0) + 1
  }
  // 写入数据到 public 目录以便 VitePress 复制到 dist
  const publicDir = path.join(OUT_DIR, 'public')
  const publicExpDir = path.join(publicDir, 'experiments')
  fs.mkdirSync(publicExpDir, { recursive: true })
  fs.writeFileSync(path.join(publicExpDir, '_data.json'), JSON.stringify(stats, null, 2))

  // 生成实验列表页
  const byLang = {}
  for (const link of expLinks) {
    if (!byLang[link.language]) byLang[link.language] = []
    byLang[link.language].push(link)
  }

  let indexContent = `---
title: 实验列表
---

# 实验列表

共 ${stats.total} 个实验
\n`

  const langOrder = Object.keys(byLang).sort()
  for (const lang of langOrder) {
    indexContent += `\n## ${lang.toUpperCase()}\n\n`
    for (const link of byLang[lang]) {
      const tags = link.tags.map(t => `\`${t}\``).join(' ')
      indexContent += `- [${link.name}](./${link.slug}.html) — ${link.description} ${tags ? `(${tags})` : ''}\n`
    }
  }

  fs.writeFileSync(path.join(expOutDir, 'index.md'), indexContent)

  // 生成各语言分类页
  for (const lang of langOrder) {
    let langContent = `---
title: ${lang.toUpperCase()} 实验
---

# ${lang.toUpperCase()} 实验

共 ${byLang[lang].length} 个实验
\n`
    for (const link of byLang[lang]) {
      const tags = link.tags.map(t => `\`${t}\``).join(' ')
      langContent += `- [${link.name}](../experiments/${link.slug}.html) — ${link.description} ${tags ? `(${tags})` : ''}\n`
    }
    fs.writeFileSync(path.join(langOutDir, `${lang}.md`), langContent)
  }

  // 生成标签页
  const allTags = new Set()
  for (const link of expLinks) {
    for (const t of link.tags) allTags.add(t)
  }
  let tagsContent = `---
title: 标签
---

# 标签
\n`
  for (const tag of [...allTags].sort()) {
    const tagged = expLinks.filter(l => l.tags.includes(tag))
    tagsContent += `\n## ${tag}\n\n`
    for (const link of tagged) {
      tagsContent += `- [${link.name}](../experiments/${link.slug}.html)\n`
    }
  }
  fs.writeFileSync(path.join(expOutDir, 'tags.md'), tagsContent)

  // 生成 sidebar.json
  const sidebarLangItems = langOrder.map(lang => ({
    text: `${lang.toUpperCase()} (${byLang[lang].length})`,
    link: `/languages/${lang}`
  }))

  const sidebarData = {
    '/experiments/': [
      { text: '全部实验', link: '/experiments/' },
      { text: '标签', link: '/experiments/tags' },
      {
        text: '按语言',
        items: sidebarLangItems
      }
    ],
    '/languages/': [
      { text: '全部实验', link: '/experiments/' },
      {
        text: '按语言',
        items: sidebarLangItems
      }
    ]
  }
  fs.writeFileSync(path.join(OUT_DIR, '.vitepress/sidebar.json'), JSON.stringify(sidebarData, null, 2))

  console.log(`Generated ${allExps.length} experiment pages`)
  console.log('Done.')
}

main()
