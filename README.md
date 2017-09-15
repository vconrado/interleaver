# Interleaver

Esse programa intercala o conteúdo de um conjunto de arquivos. 
Foi desenvolvido com o objetivo de auxiliar na preparação de dados para serem carregados no [SciDB](http://forum.paradigm4.com/).

## Instalação
```bash
git clone https://github.com/vconrado/interleaver.git
cd interleaver/build
./configure
make
make install

```

## Usando o Interleaver

Para usar o **interleaver**, chame o programa passando como argumento cada arquivo seguido pela quantidade de bytes que representa seus dados. O último parâmetro deve ser o nome do arquivo de saída.

```bash
Usage: interleaver file1 nbytes1 file2 nbytes2 [fileN nbytesN] output_file
```

Por exemplo, para intercalar dois arquivos, onde o primeiro tem dados de 1 byte e o segundo dados com 2 bytes, use:
```bash
interleaver file1 1 file2 2 file.out
```


