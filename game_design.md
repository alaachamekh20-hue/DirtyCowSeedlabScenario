# Dirty COW Attack Lab (SEED Lab on CyberRangeCZ) - Game Design Document

## 1. Overview
- **Scenario Name**: `dirtycow-seedlab`
- **Topic**: Operating System Security - Kernel Race Condition & Copy-On-Write (CVE-2016-5195)
- **Reference**: SEED Labs Dirty COW Attack Lab (Wenliang Du) & CyberRangeCZ scenario structure (`library-junior-hacker`)
- **Total Points**: 20 / 20 points
- **Student Capacity**: 30+ students (with unique dynamic flags per sandbox to prevent cheating)
- **Target Audience**: Cybersecurity students, systems security researchers, penetration testers

---

## 2. Infrastructure & Topology

The topology is designed for **CyberRangeCZ / OpenStack** using standard flavors and images:

| Host | Image | Mgmt User | Flavor | IP Address | Purpose |
|------|-------|-----------|--------|------------|---------|
| `seed` | `seed` / `seed-ubuntu1204` | `ubuntu` | `m1.medium` | `10.1.30.10` | Student workstation with exploit source files, `/zzz`, and target user `charlie` |
| `server` | `debian-12-x86_64` | `debian` | `standard.small` | `10.1.30.5` | Scoring server & flag verification API (`cow_flag/task1`, `cow_flag/task2`) |
| `router` | `debian-12-x86_64` | `debian` | `standard.small` | `10.1.30.1` | Sandbox router and default gateway |

### Pre-configured Accounts:
- Default student user: `user` / `Password123`
- Unprivileged victim user: `charlie` / `seedcharlie` (UID `1001`)
- Superuser: `root` (UID `0`)

---

## 3. Scoring Breakdown (Total 20 / 20) & Anti-Cheating Strategy

To support 30+ students simultaneously without sharing answers, the lab utilizes CyberRangeCZ's **Automatic Problem Generation (APG)** feature via `variables.yml` (`variant_sandboxes: true`). Each sandbox dynamically provisions distinct tokens for `flag_task1` and `flag_task2`.

| Level | Title | Max Score | Hints & Penalties | Flag / Answer | Anti-Cheating Mechanism |
|---|---|---|---|---|---|
| **0** | Introduction | 0 pt | None | None (Info Level) | N/A |
| **1** | Access the Sandbox | 0 pt | None | `start` (Access Level) | Initial verification |
| **2** | Task 1: Memory Mapping and COW | 3 pt | Hint 1 (-0.5 pt)<br>Hint 2 (-1.0 pt) | `MAP_PRIVATE` | Static knowledge check |
| **3** | Task 2: Modifying Dummy Read-Only File | 7 pt | Hint 1 (-1.0 pt)<br>Hint 2 (-1.5 pt)<br>Hint 3 (-2.0 pt) | `{{ flag_task1 }}` | Dynamic per-sandbox flag |
| **4** | Task 3: Analyzing /etc/passwd and Root UID | 3 pt | Hint 1 (-0.5 pt)<br>Hint 2 (-1.0 pt) | `0` | Static knowledge check |
| **5** | Task 4: Privilege Escalation via /etc/passwd | 7 pt | Hint 1 (-1.0 pt)<br>Hint 2 (-1.5 pt)<br>Hint 3 (-2.0 pt) | `{{ flag_task2 }}` | Dynamic per-sandbox flag |
| **Total** | | **20 / 20 pt** | **Penalties: -0.5 to -2.0** | | Dynamic flags prevent plagiarism |

---

## 4. Educational Workflow

1. **Memory Architecture Analysis**:
   - Students learn how the Linux memory manager optimizes process memory using Copy-On-Write (COW).
   - They explore why `mmap` with `MAP_PRIVATE` normally restricts changes to private copies.

2. **Race Condition Exploitation**:
   - Students run two concurrent threads:
     - `writeThread`: repeatedly writing to `/proc/self/mem`.
     - `madviseThread`: invoking `madvise(..., MADV_DONTNEED)` to invalidate the mapped page.
   - When a race occurs, the page table points directly back to the underlying physical page cache of the file, allowing non-root users to write to root-owned read-only files.

3. **Dummy File Proof of Concept**:
   - The student successfully alters `/zzz` from `111111222222333333` to `111111******333333`.
   - The verification server validates the write and releases the Task 1 flag.

4. **Root Privilege Escalation**:
   - The student adapts the exploit to target `/etc/passwd`.
   - Modifies `charlie:x:1001` to `charlie:x:0000`.
   - Switches to user `charlie` (`su charlie`), resulting in immediate root permissions (`uid=0(root)`).
   - Retrieves the final flag from the verification server.
