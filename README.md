დავალებასთან დაკავშირებული ყველა კითხვა მოძებნეთ issue-ებში (პირობა, ტექნიკური პრობლემა...). თუ სხვასაც ქონდა, დაამატეთ upvote ან დააკომენტარეთ მეტი დეტალი. თუ არა, შექმენით ახალი

## დავალების ატვირთვა
დავალება უნდა ატვირთოთ google classroom-ზე
### ავტომატურად
დააინსტალირეთ პროგრამა zip
გაუშვით ბრძანება `./zip.sh` დავალების დირექტორიაში. შეიყვანეთ მეილის აიდი (შემდეგი დავალებისთვის დაიმახსოვრებს^^ თუ არასწორად შეიყვანთ, შეცვალეთ ფაილში `~/.config/paradigms`. 

### manual
- დაზიპეთ ფაილები `imdb.cc six-degrees.cc`. **აუცილებელია იყოს `.zip` და არა rar**. დაზიპეთ **მხოლოდ** ეს ფაილები (სხვა ფაილებში ცვლილებები არ მიიღება) და არა დირექტორია
- დაარქვით ზიპს თქვენი მეილის id

## prerequisites 
install valgrind, zip and unzip commands
```sh
sudo apt-get install valgrind #ubuntu
yay -S valgrind #arch
```

## setup
### automatic
```sh
sh setup.sh
```

### manual
1. run chmod lines from the setup script 
2. download archive from this [link](https://github.com/freeuni-paradigms/assn-2-six-degrees-data/archive/master.zip). extract archive. move `data` folder in the assignment folder

## development

1. build: `make`

2. run: `./six-degrees`

3. test
```sh
./six-degrees-checker64 ./six-degrees
# check for memory leaks
./six-degrees-checker64 ./six-degrees -m
```
სულ არის 12 ტესტი, დავალების ქულა იქნება (წარმატებით ჩაბარებული ტესტების რაოდენობა) / 12


## ქულა
```sh
./six-degrees-checker64 ./six-degrees > output.txt
./six-degrees-checker64 ./six-degrees -m >> output.txt
cat output.txt | grep "SUCCESSFULY" | wc -l > result.txt
```
