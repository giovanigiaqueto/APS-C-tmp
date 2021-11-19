
# ===== dependências do arquivo principal =====

$(OBJDIR)/main.o: \
	$(addprefix $(SRCDIR)/, \
		main.c cmd_params.h aleatorio.h geracao_dados.h \
		algs_ordenacao.h ler_arquivo.h temporizacao.h resultados.h \
	)

# ===== dependências dos módulos auxíliares =====

$(OBJDUR)/cmd_params.o: \
	$(addprefix $(SRCDIR)/, \
		cmd_params.c cmd_params.h ler_arquivos.h algs_ordenacao.h \
	)

$(OBJDIR)/algs_ordenacao.o: \
	$(addprefix $(SRCDIR)/, \
		algs_ordenacao.c algs_ordenacao.h algoritimo.h \
		$(addprefix sort/, \
			bubble_sort.h select_sort.h quick_sort.h \
			insertion_sort.h binary_insertion_sort.h \
			merge_sort.h heap_sort.h bucket_sort.h \
		) \
	)

$(OBJDIR)/resultados.o: \
	$(addprefix $(SRCDIR)/, resultados.c resultados.h)

$(OBJDIR)/aleatorio.o: \
	$(addprefix $(SRCDIR)/, aleatorio.c aleatorio.h)

$(OBJDIR)/geracao_dados.o: \
	$(addprefix $(SRCDIR)/, geracao_dados.c geracao_dados.h aleatorio.h)

$(OBJDIR)/ler_arquivo.o: \
	$(addprefix $(SRCDIR)/, ler_arquivo.c ler_arquivo.h)

$(OBJDIR)/lista_linhas.o: \
	$(addprefix $(SRCDIR)/, lista_linhas.c lista_linhas.h)

$(OBJDIR)/temporizacao.o: \
	$(addprefix $(SRCDIR)/, temporizacao.c temporizacao.h)


# ===== dependências dos algoritimos de ordenação =====

$(OBJDIR)/insertion_sort.o: \
	$(addprefix $(SRCDIR)/, \
		insertion_sort.c insertion_sort.h algoritimo.h lista_linhas.h \
	)

$(OBJDIR)/binary_insertion_sort.o: \
	$(addprefix $(SRCDIR)/, \
		binary_insertion_sort.c binary_insertion_sort.h algoritimo.h \
	)

$(OBJDIR)/ordenacao_bolha.o: \
	$(addprefix $(SRCDIR)/, \
		ordenacao_bolha.c ordenacao_bolha.h algoritimo.h \
	)

$(OBJDIR)/quick_sort.o: \
	$(addprefix $(SRCDIR)/, \
		quick_sort.c quick_sort.h algoritimo.h \
	)

$(OBJDIR)/select_sort.o: \
	$(addprefix $(SRCDIR)/, \
		select_sort.c select_sort.h algoritimo.h \
	)

$(OBJDIR)/merge_sort.o: \
	$(addprefix $(SRCDIR)/, \
		merge_sort.c merge_sort.h algoritimo.h \
	)

$(OBJDIR)/bucket_sort.o: \
	$(addprefix $(SRCDIR)/, \
		bucket_sort.c bucket_sort.h insertion_sort.h \
		algoritimo.h lista_linhas.h \
	)

$(OBJDIR)/heap_sort.o: \
	$(addprefix $(SRCDIR)/, \
		heap_sort.c heap_sort.h heap_sort_impl.h algoritimo.h \
	)
