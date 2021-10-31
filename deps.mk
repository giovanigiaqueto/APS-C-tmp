
# ===== dependências do arquivo principal =====

$(OBJDIR)/main.o: $(SRCDIR)/main.c


# ===== dependências dos módulos auxíliares =====

$(OBJDIR)/algoritimo.o: \
	$(addprefix $(SRCDIR)/, algoritimo.c algoritimo.h lista_linhas.h)

$(OBJDIR)/ler_arquivo.o: \
	$(addprefix $(SRCDIR)/, ler_arquivo.c ler_arquivo.h)

$(OBJDIR)/temporizacao.o: \
	$(addprefix $(SRCDIR)/, temporizacao.c temporizacao.h)


# ===== dependências dos algoritimos de ordenação =====

$(OBJDIR)/insertion_sort.o: \
	$(addprefix $(SRCDIR)/, insertion_sort.c insertion_sort.h algoritimo.h lista_linhas.h)

$(OBJDIR)/ordenacao_bolha.o: \
	$(addprefix $(SRCDIR)/, ordenacao_bolha.c ordenacao_bolha.h algoritimo.h)

$(OBJDIR)/quick_sort.o: \
	$(addprefix $(SRCDIR)/, quick_sort.c quick_sort.h algoritimo.h)

$(OBJDIR)/select_sort.o: \
	$(addprefix $(SRCDIR)/, select_sort.c select_sort.h algoritimo.h)


