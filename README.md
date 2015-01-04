#  소개

[은전한닢 프로젝트][1]를 이용한 MySQL FullText Parser Plugin이다.
MySQL 5.1부터 외부 개발자가 Plugin을 개발하여 MySQL의 기능을 확장할 수 있다.Plugin 종류 중 [FullText Parser Plugin][3]을 활용하면 MySQL에서 기본 제공하는 Parser 대신 외부 개발자가 Parser를 만들 수 있다.
은전한닢 프로젝트는 MeCab(일본에서 만들어진 일본어 형태소분석기)를 기반으로 만들어진 한국어 형태소 분석기이다.

FullText Parser Plugin을 어떻게 개발하는지 용도로 간단하게 만들어 봤습니다. 소스 코드도 얼마 안 된다. 기술력 좋은 검색 엔진 회사라면 성능 및 형태소 분석 능력이 좋은 Plugin을 쉽게 개발하실 수 있습니다. 한글을 처리하기 위해 일본에서 만든 MeCab을 사용해야 한다는 것이 아쉽긴 합니다만 이것이 우리의 현실임....

# 각종 관련 자료들

* [은전한닢 프로젝트 소개 slide][4] : 은전한닢에 대한 소개와 사용 방법을 보실 수 있습니다. 은전한닢 프로젝트 담당자들이 작성한 글.
* [MySQL Plugin에 대한 간략한 정리][5] : MySQL Plugin에 대해서 아주 간략하게 정리해 봤습니다. 제가 안 까먹으려고 적은 것이라서 매우 간단함.
* [FullText Plugin에 대한 간략한 정리][6] : MySQL Plugin 중 FullText Parser Plugin에 대해서만 정리해 봤습니다. 제가 안 까먹으려고 적은 것이라서 매우 간단함.
* [MySQL 5.1 Plugin Development][7] : MySQL에 Plugin framework을 만든 개발자들이 직접 집필한 책입니다. Plugin에 대해서 자세히 공부하고 싶은 분은 이 책을 꼭 보시기 바란다.

# 한계

## FullText Parser Plugin의 한계

다음은 MySQL에서 FullText Parser Plugin의 한계이다.

* MySQL 5.6 이하에서는 MyISAM Table에서만 사용할 수 있다.
* MySQL 5.6에서는 InnoDB에서 FullText 기능을 사용할 수 있지만, Plugin 형식의 FullText는 MyISAM에서만 사용할 수 있다.
* MySQL 5.7.3 부터 InnoDB에서 FullText Parser Plugin을 사용할 수 있다.

## 본인이 만든 Plugin의 한계

* `IN BOOLEAN MODE`를 지원하지 않는다. 이 부분은 문서를 좀 더 읽어보고 시간이 되면 지원할 예정이다.
* 문자셋을 `utf8`만 지원한다.

# 설치 및 사용 방법

## Test 환경

* MySQL 5.5
* CentOS 6.3

## 요약

- MeCab을 설치. MeCab 설치는 다음과 같이 2단계로 나뒨다.
 - MeCab 설치
 - 한글 사전(dic) 설치
- 한글 FullText Parser Plugin을 다운받는다.
- 컴파일 한다. 설치한다. 사용한다.

## 1) MeCab 설치

본인은 [은전한닢 프로젝트 소개 slide][4]를 보고 설치 방법을 참고했다.

### 1-1) MeCab 설치

형태소 분석기 자체를 설치하는 단계로서 먼저 다음 파일을 다운받는다.

```
$ wget https://bitbucket.org/eunjeon/mecab-ko/downloads/mecab-0.996-ko-0.9.1.tar.gz
$ tar xvfz mecab-0.996-ko-0.9.1.tar.gz
$ cd mecab-0.996-ko-0.9.1/
$ ./configure --prefix=/path/to/mecab
$ make
$ make install
```

### 1-2) 한글 사전(dic) 설치

형태소 분석에 사용될 사전을 설치하는 단계로서 [은전한닢 프로젝트 홈][1]을 방문하시면 종종 사전 업데이트 소식이 올라온다. 형태소 분석에 가장 중요한 것이 바로 사전이므로 가급적 최신 버전을 설치하길 바란다.


```
$ wget https://bitbucket.org/eunjeon/mecab-ko-dic/downloads/mecab-ko-dic-1.6.1-20140814.tar.gz
$ tar xvfz mecab-ko-dic-1.6.1-20140814.tar.gz
$ cd mecab-ko-dic-1.6.1-20140814/
$ ./configure --with-mecab-config=/path/to/mecab/bin/mecab-config
$ make
$ make install
```

### 1-3) 설정

#### 사전(dic) 경로 설정

`/path/to/mecab/etc/mecabrc`에 `dicdir`을 다음과 같이 수정

```
dicdir =  /path/to/mecab/lib/mecab/dic/mecab-ko-dic/
```

#### `LD_LIBRARY_PATH`, `PATH` 등록

`bash` 계열의 경우 `$HOME/.bashrc`에 다음 내용 추가

```
export PATH=/path/to/mecab/bin/:$PATH
export LD_LIBRARY_PATH=/path/to/mecab/lib/:$LD_LIBRARY_PATH
```

#### MySQL restart

MySQL이 이미 시작 중이었다면 재시작해 주자. `LD_LIBRARY_PATH`가 변경된 것을 반영하기 위함이다. MySQL 재시작 방법은 각자 환경에 따라 다르다.

### 1-4) Test

MeCab이 잘 작동하는지 다음과 같이 테스트해보자

```
$ echo "동해물과백두산이 마르고 닳도록. 독도는우리땅" | mecab
동해  NNP,지명,F,동해,*,*,*,*,*
물     NNG,*,T,물,*,*,*,*,*
과     JC,*,F,과,*,*,*,*,*
백두산       NNP,지명,T,백두산,Compound,*,*,백두+산,백두/NNG/*/1/1+백두산/Compound/*/0/2+산/NNG/*/1/1
이     JKS,*,F,이,*,*,*,*,*
마르  VV,*,F,마르,*,*,*,*,*
고     EC,*,F,고,*,*,*,*,*
닳     VV,*,T,닳,*,*,*,*,*
도록  EC,*,T,도록,*,*,*,*,*
.       SY,*,*,*,*,*,*,*,*
독도  NNP,지명,F,독도,*,*,*,*,*
는     JX,*,T,는,*,*,*,*,*
우리  NP,*,F,우리,*,*,*,*,*
땅     NNG,*,T,땅,*,*,*,*,*
EOS
```

"MM", "NNG", "JC" 같은 것을 "품사"라고 하며 [품사 종류는 여기][8]서 확인할 수 있다.

MeCab의 경우 "동해물"을 "동해"와 "물"로 분석을 하였다. 어떤 것이 정답인지 정의하긴 어렵지만 "동해물"로 분석하는 것이 맞다면 MeCab의 한글 사전을 좋게 만들어야 한다. 현재 80만건 정도의 단어가 존재하며 부족한 수준이지만 이를 개인 프로젝트의 노력만으로 사전을 풍부하게 하긴 어려울 것이다.

"사전"에 단어를 직접 추가할 수 있으며 역시 [은전한닢 프로젝트 소개 slide][4]에서 볼 수 있다.

## 2) 한글 FullText Parser Plugin 컴파일

다음 방법으로 한글 FullText Parser Plugin을 컴파일 한다.

`mysql_config` 및 `mecab-config`가 `PATH`에 등록되어 있어야 한다. 흔히들 소스 컴파일할 때 사용하는 `./configure`는 본인이 autotools 사용 방법을 몰라서 생략했다. 위의 두 파일이 없는 경우 컴파일에 오류가 생긴다.

```
$ git clone https://github.com/mysqlguru/mysql-ko-ftparser
$ cd mysql-ko-ftparser/
$ make
```

컴파일이 완료되었으면 간단한 테스트 프로그램을 실행하여 정상작동 여부를 확인해보자.

```
$ ./mecab_test
동해물과백두산이 마르고 닳도록
- 동해
- 물
- 백두산
---- end of result ----
독도는우리땅
- 독도
- 우리
- 땅
---- end of result ----
```

워낙 간단히 구현하다보니 에러 처리를 완벽하게 하지 않았다. 예를 들어 사전 경로를 잘못 지정 한 경우 다음과 같은 에러를 출력하고 종료한다.

```
$ ./mecab_test
terminate called after throwing an instance of 'std::runtime_error'
  what():  init failed:
중지됨 (core dumped)

```

마지막으로 `make install`을 입력하면 Plugin이 MySQL의 plugin을 저장하는 path로 복사된다.

```
$ make install
cp mysql-ko-ftparser.so `mysql_config --plugindir`
```

## 3) MySQL Plugin 설치 및 사용 방법

### 3-1) Plugin 설치 방법

```
mysql> INSTALL PLUGIN mysql_ko_ftparser SONAME 'mysql-ko-ftparser.so';
Query OK, 0 rows affected (0.00 sec)
```

### 3-2) 사용 예

`test_native`는 MySQL의 자체 FullTest Parser를 사용하는 테이블이며, `test_ko`는 한글 FullText Parser Plugin을 이용하는 테이블이다.

```
DROP TABLE IF EXISTS test_native;
DROP TABLE IF EXISTS test_ko;

CREATE TABLE test_native
(
    id INT AUTO_INCREMENT NOT NULL,
    content TEXT,
    PRIMARY KEY(id),
    FULLTEXT(content)
) ENGINE = MyISAM CHARSET = utf8;

CREATE TABLE test_ko
(
    id INT AUTO_INCREMENT NOT NULL,
    content TEXT,
    PRIMARY KEY(id),
    FULLTEXT(content) WITH PARSER mysql_ko_ftparser
) ENGINE = MyISAM CHARSET = utf8;
```

필자는 다음과 같은 Data를 입력했다. FullText 컬럼에 UPDATE를 하든 INSERT를 하든 값이 변경되는 경우 자동으로 FullText Parser가 작동한다.

```
INSERT INTO test_native (content) VALUES
    ('동해물과백두산이 마르고 닳도록'),
    ('독도는 우리 땅'),
    ('대한민국의 주권은 국민에게 있고, 모든 권력은 국민으로부터 나온다');

INSERT INTO test_ko SELECT * FROM test_native;
```

`myisam_ftdump` 명령을 이용하면 FullText 컬럼에 어떤 문자열이 indexing되었는지 확인 할 수 있다. 다음 명령은 MySQL Data directory의 해당 DB directory에서 실행시켜야 한다.

MySQL 자체 FullText는 다음과 같은 문자열이 indexing되어있다.

```
$ myisam_ftdump test_native 1 --dump
       58            0.9666505 국민에게
       58            0.9666505 국민으로부터
       58            0.9666505 대한민국의
        0            0.9886308 동해물과백두산이
```

한글 FullText Parser Plugin을 사용한 경우는 다음과 같다.

```
$ myisam_ftdump test_ko 1 --dump
$ myisam_ftdump test_ko 1 --dump
       58            1.3796179 국민
       58            0.8148246 권력
       58            0.8148246 대한민국
       38            0.9666505 독도
        0            0.9666505 동해
       38            0.9666505 땅
        0            0.9666505 물
        0            0.9666505 백두산
       38            0.9666505 우리
       58            0.8148246 주권
```

결과를 보면 알 수 있겠지만, '백두산'으로 검색한 경우 `test_ko`에서만 결과가 출력된다.

```
mysql> SELECT id, content
    -> FROM test_native WHERE MATCH content AGAINST ('백두산')\G
Empty set (0.00 sec)

mysql> SELECT id, content
    -> FROM test_ko WHERE MATCH content AGAINST ('백두산')\G
*************************** 1. row ***************************
     id: 1
content: 동해물과백두산이 마르고 닳도록
1 row in set (0.00 sec)
```

### 3-3) Plugin 삭제

다음 구문을 이용하여 MySQL에서 Plugin을 삭제할 수 있다.

```
mysql> UNINSTALL PLUGIN mysql_ko_ftparser;

```

### 3-4) Parsing 속도를 높히는 방법

테스트 결과 네이버 뉴스 본문을 INSERT하는데 400ms 정도가 소요된다. 하지만 동일 뉴스를 다시 INSERT하면 10ms 정도만 소요되는 것을 보아 사전 Data를 읽는 IO가 많은 것으로 예상된다.

다음과 같은 명령을 실행하여 사전 Data를 Memory로 Caching해 두면 INSERT 속도가 향상된다.

```
$ cat /path/to/mecab/lib/mecab/dic/mecab-ko-dic/* > /dev/null
```

물론 서버 운영 중 Page Cache가 Disk로 내려가게 되면 INSERT 속도가 느려질 수 있으므로 항상 메모리에 올라와 있을 수 있도록 운영의 묘를 잘 살리도록 하자.

[1]: http://eunjeon.blogspot.kr/
[2]: http://dev.mysql.com/doc/refman/5.5/en/plugin-api.html
[3]: http://dev.mysql.com/doc/refman/5.5/en/full-text-plugins.html
[4]: http://eunjeon.blogspot.kr/2014/06/blog-post.html
[5]: http://mysqlguru.github.io/mysql/2014/12/17/mysql-plugin-development.html
[6]: http://mysqlguru.github.io/mysql/2014/12/30/mysql-fulltext-plugin.html
[7]: http://www.amazon.com/MySQL-Plugin-Development-Sergei-Golubchik/dp/1849510601
[8]: https://docs.google.com/spreadsheets/d/1-9blXKjtjeKZqsf4NzHeYJCrr49-nXeRF6D80udfcwY/edit#gid=0
