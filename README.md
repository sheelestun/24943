# 24943

Подключение к Solaris

https://github.com/vkinsu/

ssh username@ccfit.nsu.ru

ssh-keygen -t rsa

cp .ssh/id_rsa.pub .ssh/authorized_keys

ssh 10.4.0.68

cat .ssh/id_rsa.pub

# Первый этап

1. Перейти по ссылке https://github.com/vkinsu/24943.git
2. В правом верхнем углу нажать на Fork -> Create new fork...
3. Теперь в профиле есть копия этого репозитория


# Второй этап
Если вы  используете linux или на Windows используете Git Bash, то можно пойти таким путем:
1. После того как был создан fork перейдите в свой аккаунт github
2. Затем нажмите на кнопку clone там будет ссылка на ваш репозиторий
3. Далее в терминале пишете следующую команду где ссылка это то что вы скопировали на предыдущем шаге:
`git clone ссылка`
4. теперь на вашей машине есть копия репозитория 



# Третий этап
Возвращаемся на github.

1. Нажимаем на кнопку справа сверху где есть иконка вашего профиля и заходим в settings
   
![sample](https://github.com/alexmihalyk23/NSU-OS-22930/assets/35634279/d32d8aff-f1b0-4c5d-8c13-91eb713f6e69)

2. Затем нажимаем на кнопку SSH and GPG keys 

![ssh keys](https://github.com/alexmihalyk23/NSU-OS-22930/assets/35634279/637d4072-bcd8-47c3-8a47-c3ea84177114)




3. Нажимаем на кнопку New ssh key
4. На вашей машине открываете id_rsa.pub и копируете весь текст
5. После этого вставляете его в поле ключа на github


В папке с вашим репозиторием находится папка .git, в которой есть файл config в нем необходимо заменить строчку url
на  git@github.com:ваше имя аккаунта/vkinsu/24943.git  



# Четвертый Этап (работа с git)

1. git init для инициализации репозитория
2. git add <Путь к вашей папке> добавление в репозиторий
3. git commit -m "<Что-то написать>" сделать коммит
4. git push для загрузки на github
# 24943


#…or create a new repository on the command line
1. git init
2. git add README.md
3. git commit -m "first commit"
4. git branch -M main
5. git remote add origin git@github.com:vkinsu/24940.git
6. git push -u origin main

#…or push an existing repository from the command line
1. git remote add origin git@github.com:vkinsu/24940.git
2. git branch -M main
3. git push -u origin main

