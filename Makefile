all: sample/config.m4 sample-ext-build

sample-ext-build: php_ext_embed.c php_ext_embed.h
	@$(MAKE) -C sample

sample/config.m4: php_ext_embed.m4

php_ext_embed.m4:
	cd sample && phpize && ./configure

test: all
	@$(MAKE) -C sample test

clean: sample/config.m4
	@$(MAKE) -C sample clean
