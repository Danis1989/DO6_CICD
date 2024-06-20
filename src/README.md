# Basic CI/CD

1. [Настройка gitlab-runner](#part-1-настройка-gitlab-runner)  
2. [Сборка](#part-2-сборка)  
3. [Тест кодстайла](#part-3-тест-кодстайла)   
4. [Интеграционные тесты](#part-4-интеграционные-тесты)  
5. [Этап деплоя](#part-5-этап-деплоя)  
6. [Дополнительно. Уведомления](#part-6-дополнительно-уведомления)

## Part1. Настройка gitlab-runner

* Скачать и установить на виртуальную машину gitlab-runner  
    - Добавил официальный репозиторий GitLab:  
    `curl -L "https://packages.gitlab.com/install/repositories/runner/gitlab-runner/script.deb.sh"`
    - Установка:  
    `sudo apt-get install gitlab-runner`
* Запустить gitlab-runner и зарегистрировать его для использования в текущем проекте (DO6_CICD)  
    - Команда начала регистрации:  
    `gitlab-runner register`
    - Процесс регистрации(введены URL и токен проекта, в качестве executor был выбран `shell`):  
    ![reg](src/01/pt1.1.png)  

## Part2. Сборка
В файле `.gitlab-ci.yml` прописываем конфигурацию цели build:  
![build](src/02/pt2.1.png) 

`build-job` - название цели;  
`stage` - в каком stage она будет выполняться;  
`script` - команды для выполнения;  
`artifacts` - промежуточные результаты, которые сохраняются в gitlab ci;  
`expire_in` - время хранения артефактов;  
`paths` - места расположения артефактов;  

Результат работы цели `build-job`, запущена сборка проекта через мейк файл и получены артефакты:  
![build](src/02/pt2.2.png)  

## Part3. Тест кодстайла

В файле `.gitlab-ci.yml` прописываем конфигурацию цели code_style:  
![code_style_OK](src/03/pt3.1.png)

Результат работы корректного прохождения тестов:  
![code_style_OK](src/03/pt3.2.png)  

Пример заваленного `Pipeline` после ошибки clang-format:  
![code_style_FAIL](src/03/pt3.3.png)  

## Part4. Интеграционные тесты

В файле `.gitlab-ci.yml` прописываем конфигурацию цели functional_test:
  
![functional_test_OK](src/04/pt4.1.png)

Результат работы корректного прохождения тестов: 
 
![functional_test_OK](src/04/pt4.2.png)  

Пример заваленного `Pipeline` после ошибки тестов:
  
![functional_test_FAIL](src/04/pt4.3.png)  

## Part5. Этап деплоя

Создана вторая машина и проведён статический маршрут через сетевой адаптер `enp0s8`:  
![tunnel](src/05/pt5.1.png)  ![tunnel](src/05/pt5.2.png) 
![tunnel](src/05/pt5.3.png)  ![tunnel](src/05/pt5.4.png)

**На первой машине:**  
`sudo su gitlab-runner` - заходим под пользователем gitlab-runner, так как gitlab ci всё делает от него 
`ssh-copy-id -i /home/gitlab-runner/.ssh/id_rsa lashawnd@172.24.116.8` - копируем ssh ключ на вторую машину для пользователя lashawnd  
 ![tunnel](src/05/pt5.5.png)
 
**На второй машине:**  
`sudo su root` - заходим под пользователем root  
`cd /root/.ssh/ && cp /home/lashawnd/.ssh/authorized_keys authorized_keys` - копируем файл authorized_keys от abalonef к root (там лежит ключ с первой машины) 
![tunnel](src/05/pt5.6.png)
  
В файле `/etc/ssh/sshd_config` необходимо разблокировать следующие параметры:  
`permitrootlogin prohibit-password` - разрешает подключение по ssh ключу  
`AuthorizedKeysFile` - файл, где лежит ssh ключ  
![sshd_config](src/05/pt5.7.png)  

*Теперь gitlab-runner с первой машины может подключаться к пользователю root на второй, который в свою очередь имеет доступ к папке /usr/local/bin*  
В файле `.gitlab-ci.yml` прописываем конфигурацию цели deploy:  
>deploy-job:  
  stage: deploy  
  allow_failure: false  
  when: manual  
  script:  
    - echo "Start deploy job"  
    - chmod +x src/copy.sh  
    - bash src/copy.sh  

`when: manual` - тестирование остановится перед началом этой цели для ручного подтверждения  
![deploy](src/05/pt5.8.png)  

## Part6. Уведомления

В файле `.gitlab-ci.yml` прописываем конфигурацию цели notify:  
>notify_fail-job:  
  stage: notify  
  script:  
    - chmod +x src/telegram_bot_conf.sh  
    - bash src/telegram_bot_conf.sh Job_Error ❌  
  when: on_failure  
  
>notify_ok-job:  
  stage: notify  
  script:  
    - bash src/telegram_bot_conf.sh Success ✅  
  when: on_success  

 `when: on_failure` - будет выполняться, если хотя бы один job зафейлен  
 `when: on_success` - будет выполняться, если все jobs успешно выполнились  

**Пример успешного pipeline:**  
![pipeline_ok](src/06/pt6.1.png)  

**Пример зафейленного pipeline:**  
![pipeline_fail](src/06/pt6.2.png)  
