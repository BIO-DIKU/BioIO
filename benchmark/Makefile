SILVA_FTP      = ftp://ftp.arb-silva.de/release_123/Exports/SILVA_123_SSURef_tax_silva_trunc.fasta.gz
SILVA_LOCAL    = silva/$(notdir $(SILVA_FTP))
SILVA_UNPACKED = $(subst .gz,,$(SILVA_LOCAL))

SEQTK_GIT      = https://github.com/lh3/seqtk.git

all: $(SILVA_UNPACKED) seqtk

$(SILVA_LOCAL):
	mkdir -p silva
	wget -O $(SILVA_LOCAL) $(SILVA_FTP)

$(SILVA_UNPACKED): $(SILVA_LOCAL)
	gunzip -k $(SILVA_LOCAL)

seqtk:
	git clone $(SEQTK_GIT) $@
	cd $@ && make


bench: seqtk $(SILVA_LOCAL)
	time ./seqtk/seqtk seq $(SILVA_LOCAL) > /dev/null

count_a: count_a.cc ../build/libBioIO.a
	g++ -g -std=c++11 -I../include $^ -o $@ 

clean:
	rm -rf $(SILVA_UNPACKED) seqtk
	@echo "Make refrained from removing $(SILVA_LOCAL). Remove this manually"
