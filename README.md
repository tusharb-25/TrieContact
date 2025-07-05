# 📞 TrieContact

**TrieContact** is a fast and efficient C++ console-based contact management system built using Trie data structures for name and number prefix-based search and autocompletion.

<div align="center">
  <img src="https://img.shields.io/badge/Language-C++-blue.svg" />
  <img src="https://img.shields.io/badge/Trie-DataStructure-success" />
  <img src="https://img.shields.io/badge/Console%20App-Interactive-brightgreen" />
</div>

---

## 🚀 Features

- 📇 Add, update, delete contacts
- 🔍 Fast prefix search with **autocomplete suggestions**
- 📱 Search by name or number
- 🧠 Efficient dual Trie storage (name & number)
- 🧹 Delete all contacts
- 📊 Show total contacts & validate entries

---

## 🧠 How It Works

- Two Trie structures:
  - `triename`: Stores names character by character
  - `trienum`: Stores numbers digit by digit
- Maps used to ensure uniqueness and enable quick reverse lookups
- DFS-based autocomplete using Trie traversal

---

## 📦 Setup & Run

### Prerequisites:
- `g++` (C++ compiler) installed

### Build and Run:
```bash
git clone https://github.com/your-username/TrieContact.git
cd TrieContact/src
g++ triecontact.cpp -o triecontact
./triecontact
