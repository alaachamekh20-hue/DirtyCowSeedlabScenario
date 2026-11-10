# Dirty COW SeedLab Scenario for CyberRangeCZ Platform

This repository contains a full **CyberRangeCZ / KYPO-compatible training scenario** implementing the **SEED Labs Dirty COW Attack Lab** (CVE-2016-5195) described in `Dirty_COW.pdf`.

It is modeled after the official [`library-junior-hacker`](https://github.com/cyberrangecz/library-junior-hacker) template.

---

## 🖥️ Infrastructure & Topology

- **Topology File**: [`topology.yml`](topology.yml)
- **Workstation Flavor**: `m1.medium`
- **Workstation Image**: `seed` (or `seed-ubuntu1204` / vulnerable Linux kernel < 4.8.3)
- **Scoring Server Image**: `debian-12-x86_64` (flavor `standard.small`, IP `10.1.30.5`)
- **Router Image**: `debian-12-x86_64` (flavor `standard.small`, IP `10.1.30.1`)

---

## 🎯 Grading & Scoring (20 / 20 Points Total)

The scenario is scored out of **20 points**, with hint penalties ranging from **-0.5 to -2.0 points**:

- **Task 1: Memory Mapping and Copy-On-Write** - `3 points` (Hint penalties: -0.5, -1.0)
- **Task 2: Modifying Dummy Read-Only File** - `7 points` (Hint penalties: -1.0, -1.5, -2.0)
- **Task 3: Analyzing /etc/passwd and Root UID** - `3 points` (Hint penalties: -0.5, -1.0)
- **Task 4: Privilege Escalation via /etc/passwd** - `7 points` (Hint penalties: -1.0, -1.5, -2.0)

**Total = 20 / 20 Points**

---

## 🛡️ Anti-Cheating Strategy for 30+ Students

To prevent students from sharing flags:
- Utilizes the CyberRangeCZ **Automatic Problem Generation (APG)** feature via [`variables.yml`](variables.yml) with `variant_sandboxes: true`.
- Each student sandbox generates unique alphanumeric tokens for `flag_task1` and `flag_task2`.
- Flag verification is served dynamically by the internal sandbox service (`flag_server.py` on `10.1.30.5:8080`), ensuring students perform the actual exploitation steps to retrieve their unique flags.

---

## 🚀 Deployment Instructions

1. Push this directory to your CyberRangeCZ scenario repository:
   ```bash
   git init
   git add .
   git commit -m "Dirty COW SeedLab scenario"
   git remote add origin <your-git-url>
   git push -u origin master
   ```
2. In the CyberRangeCZ Web Interface:
   - Go to **Sandboxes** -> **Sandbox Definitions** -> **Add Definition** (point to repo with `topology.yml` and `provisioning/`).
   - Go to **Trainings** -> **Training Definitions** -> **Import** -> Select `training.json`.
3. Create a **Training Instance** with a pool of sandboxes for your class.
