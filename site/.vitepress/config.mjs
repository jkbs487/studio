import { defineConfig } from 'vitepress'
import sidebarData from './sidebar.json' with { type: 'json' }

export default defineConfig({
  title: 'Studio - AI原生代码实验室',
  description: '研究最新版本编程语言特性的实验室代码库',
  base: '/studio/',
  lang: 'zh-CN',
  ignoreDeadLinks: true,

  themeConfig: {
    nav: [
      { text: '首页', link: '/' },
      { text: '实验', link: '/experiments/' },
      { text: '关于', link: '/about' }
    ],

    sidebar: sidebarData,

    socialLinks: [
      { icon: 'github', link: 'https://github.com/jiangkun05/studio' }
    ],

    footer: {
      message: '基于 MIT 许可发布',
      copyright: 'Copyright © 2026 Studio Lab'
    },

    search: {
      provider: 'local'
    }
  },

  head: [
    ['link', { rel: 'icon', href: '/studio/favicon.ico' }]
  ]
})
