install-exec-local:
	$(mkinstalldirs) -m 755 $(DESTDIR)@CONFDIR@
	install -b -m 744 passgen.conf $(DESTDIR)@CONFDIR@/passgen.conf
	
uninstall-local:
	-rm -rf $(DESTDIR)@CONFDIR@/passgen.conf

