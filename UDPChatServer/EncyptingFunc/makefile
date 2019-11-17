



all : ../libencrypt.so

../libencrypt.so : encryptor/encrypt.c
	gcc -o libencrypt.so encryptor/encrypt.c -fPIC -shared -g
	mv libencrypt.so ../.

clean :
	rm ../libencrypt.so
