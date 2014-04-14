all: sample-ext

sample-ext: php_ext_embed.c php_ext_embed.h sample/config.m4
	@$(MAKE) -C sample

sample/config.m4: php_ext_embed.m4
	cd sample && phpize && ./configure

test: all
	@$(MAKE) -C sample test

clean: sample/config.m4
	@$(MAKE) -C sample clean
