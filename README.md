### Pull Experiment 3

```
wget https://raw.githubusercontent.com/AgiGames/CD/main/exp3/{lexical-analyzer.l,file.cpp}
```

Compile & Run
```
flex lexical-analyzer.l && gcc lex.yy.c && ./a.out
```

---

### Pull Experiment 4

```
wget https://raw.githubusercontent.com/AgiGames/CD/main/exp4/count-vowels.l
```

Compile & Run
```
flex count-vowels.l && gcc lex.yy.c && ./a.out
```

---

### Pull Experiment 5

```
wget https://raw.githubusercontent.com/AgiGames/CD/main/exp5/{left-factoring.c,left-recursion.c}
```

Compile & Run
```
gcc left-factoring.c && ./a.out
```
```
gcc left-recursion.c && ./a.out
```

---

### Pull Experiment 6

```
wget https://raw.githubusercontent.com/AgiGames/CD/main/exp6/{calc.l,calc.y}
```

Compile & Run
```
flex calc.l && bison -d calc.y && gcc calc.tab.c lex.yy.c -lm && ./a.out
```

---

### Pull Experiment 7

```
wget https://raw.githubusercontent.com/AgiGames/CD/main/exp7/first-follow.c
```

Compile & Run
```
gcc first-follow.c && ./a.out
```

---

### Pull Experiment 8

```
wget https://raw.githubusercontent.com/AgiGames/CD/main/exp8/{first-follow.c,first-follow.h,helper.c,helper.h,left-factoring.c,left-factoring.h,left-recursion.c,left-recursion.h,main.c}
```

Compile & Run
```
gcc first-follow.c helper.c left-factoring.c left-recursion.c main.c -o && ./a.out
```
