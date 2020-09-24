არ დაგავიწყდეთ, რომ ტერმინალში შეყვანილ ბრძანებებში ნავიგაცია შესაძლებელია up/down arrow key-ებით.


## დავალების ატვირთვა
### ავტომატურად
დააინსტალირეთ პროგრამა zip
გაუშვით ბრძანება `sh zip.sh` დავალების დირექტორიაში. შეიყვანეთ მეილის აიდი (შემდეგი დავალებისთვის დაიმახსოვრებს^^ თუ არასწორად შეიყვანთ, შეცვალეთ ფაილში `~/.config/paradigms`. 

### manual
- დაზიპეთ ფაილები `imdb.cc six-degrees.cc`. **აუცილებელია იყოს `.zip` და არა rar**. დაზიპეთ **მხოლოდ** ეს ფაილები და არა დირექტორია
- დაარქვით ზიპს თქვენი მეილის id

## setup
0. install valgrind
```sh
sudo apt-get install valgrind #ubuntu
yay -S valgrind #arch
```

change script permissions
```sh
chmod u+x rsgChecker*
chmod u+x rsg-sample*
```

1. build: `make`

2. run: `./six-degrees`

3. test
```sh
./six-degrees-checker64 ./six-degrees
# to test on memory leaks run with -m
./six-degrees-checker64 ./six-degrees -m
```

