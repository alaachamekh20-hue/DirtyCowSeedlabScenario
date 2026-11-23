#!/bin/bash
# Dirty COW SeedLab Local Task Checker

echo "=============================================="
echo "    Dirty COW Lab Task Verification Script    "
echo "=============================================="

echo ""
echo "[*] Task 2 Check: Overwrite /zzz"
if [ -f /zzz ]; then
    if grep -q "111111\*\*\*\*\*\*333333" /zzz 2>/dev/null; then
        echo "[+] SUCCESS: /zzz successfully modified with '******'!"
        echo -n "[+] Flag: "
        curl -s http://10.1.30.5:8080/cow_flag/task1 || echo "FLAG{DIRTYCOW_DUMMY_WRITE_SUCCESS}"
        echo ""
    else
        echo "[-] Not completed yet. Current contents of /zzz:"
        cat /zzz
    fi
else
    echo "[-] /zzz file does not exist."
fi

echo ""
echo "[*] Task 4 Check: Modify charlie UID in /etc/passwd"
if grep -q "charlie:x:0000" /etc/passwd 2>/dev/null; then
    echo "[+] SUCCESS: charlie UID has been changed to 0000 in /etc/passwd!"
    echo -n "[+] Flag: "
    curl -s http://10.1.30.5:8080/cow_flag/task2 || echo "FLAG{DIRTYCOW_ROOT_PRIV_ESC_PWNED}"
    echo ""
else
    echo "[-] Not completed yet. Current charlie record in /etc/passwd:"
    grep charlie /etc/passwd || echo "charlie not found in /etc/passwd"
fi
echo "=============================================="
