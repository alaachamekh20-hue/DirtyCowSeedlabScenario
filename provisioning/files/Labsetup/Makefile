all: cow_attack cow_attack_passwd

cow_attack: cow_attack.c
	gcc -O2 cow_attack.c -lpthread -o cow_attack

cow_attack_passwd: cow_attack_passwd.c
	gcc -O2 cow_attack_passwd.c -lpthread -o cow_attack_passwd

clean:
	rm -f cow_attack cow_attack_passwd
