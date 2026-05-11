---
layout: home

hero:
  name: "Studio"
  text: "AI原生代码实验室"
  tagline: 探索 C++20/23、Python 3.12+、Rust 等语言的最新特性
  actions:
    - theme: brand
      text: 浏览实验
      link: /experiments/
    - theme: alt
      text: 关于项目
      link: /about

features:
  - title: C++ 实验
    icon: ⚡
    details: 探索 C++20/23 新特性：Ranges、协程、std::expected、无锁队列等
  - title: Python 实验
    icon: 🐍
    details: 从生成器到模式匹配，深入了解 Python 3.12+ 的现代特性
  - title: Go 实验
    icon: 🚀
    details: goroutine、channel、select，掌握 Go 并发编程核心
  - title: AI 原生设计
    icon: 🤖
    details: 专为 AI 辅助开发设计，模块化实验结构，高效协作
---

## 实验统计

<script setup>
import { onMounted, ref } from 'vue'
const stats = ref({ total: 0, languages: {} })
onMounted(async () => {
  try {
    const res = await fetch('/studio/experiments/_data.json')
    const data = await res.json()
    stats.value = data
  } catch (e) {
    stats.value = { total: 0, languages: {} }
  }
})
</script>

<div class="stats-grid">
  <div class="stat-card">
    <div class="stat-number">{{ stats.total }}</div>
    <div class="stat-label">实验总数</div>
  </div>
  <div v-for="(count, lang) in stats.languages" :key="lang" class="stat-card">
    <div class="stat-number">{{ count }}</div>
    <div class="stat-label">{{ lang.toUpperCase() }}</div>
  </div>
</div>

<style>
.stats-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(120px, 1fr));
  gap: 1rem;
  margin-top: 2rem;
}
.stat-card {
  text-align: center;
  padding: 1.5rem;
  background: var(--vp-c-bg-soft);
  border-radius: 12px;
}
.stat-number {
  font-size: 2rem;
  font-weight: bold;
  color: var(--vp-c-brand);
}
.stat-label {
  margin-top: 0.5rem;
  font-size: 0.9rem;
  color: var(--vp-c-text-2);
}
</style>
