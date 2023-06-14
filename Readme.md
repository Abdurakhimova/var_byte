# VarByte

## Задача 
Реализация архивации данных в виде последовательности целых чисел кодом VarByte (составная часть библиотеки google protobuf). 
Кодирование происходит с гранулярностью в 1 байт. Признак окончания очередного закодированного числа – это первый (знаковый) бит s байта, если s=1, конец числа. Если число <=127, кодируем его 1м байтом. Иначе сохраняем число в несколько байт, каждый из которых хранит очередной остаток от деления кодируемого числа на 128.

## Запуск
Архивация:
```bash
./main <filename> -a
```
разархивировать:
```bash
./main <filename> -d
```
## Тест

```bash
./main test.bin -a
```

```bash
./main test.bin.arch -d
```

test.bin - 37 Kb
test.bin.arch - 26 Kb