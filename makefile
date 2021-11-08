
# pastas com dos códigos e de compilação
OBJDIR=obj
SRCDIR=src
INCDIR=inc

# parâmetros do compilador
CFLAGS=-Wall -std=c11 -I$(INCDIR)/

# modulos e executável principal
modulos=$(addprefix $(OBJDIR)/, \
	main.o ler_arquivo.o lista_linhas.o temporizacao.o geracao_dados.o \
	$(addprefix sort/, \
		bubble_sort.o select_sort.o quick_sort.o insertion_sort.o \
		binary_insertion_sort.o merge_sort.o bucket_sort.o \
	) \
)
alvo=main

# regra principal de compilação
all: $(alvo)

# regra para compilar o programa para depuração
debug: CFLAGS+=-g
debug: all

# regra de compilação do executável
$(alvo): $(modulos)
	@echo "linkando $(notdir $(alvo))"
	@$(CC) -o $(alvo) $(modulos) $(CFLAGS)

# regra de compilação dos modulos
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "compilando $(patsubst $(SRCDIR)/%,%,$<)"
	@$(CC) -c -o $@ $< $(CFLAGS)

# adiciona regras de dependencia entre arquivos
include deps.mk

# regra de limpeza da pasta de compilação de modulos
limpar:
	@echo "removendo objetos de compilação"
	@$(RM) -f $(modulos)
