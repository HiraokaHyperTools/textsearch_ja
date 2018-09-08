# textsearch_ja

[textsearch_ja : 形態素解析方式 全文検索](https://web.archive.org/web/20170423100313/http://textsearch-ja.projects.pgfoundry.org/textsearch_ja.html) を、PostgreSQL 9.4 - 10 Windows 版で楽しめるように助力いたします。

## インストール方法

[Releases](https://github.com/HiraokaHyperTools/textsearch_ja/releases) から `20180908.7z` をダウンロードして展開しておきます。

[MeCab](http://taku910.github.io/mecab/) をインストールします。

[mecab-0.996.exe](https://drive.google.com/uc?export=download&id=0B4y35FiV1wh7WElGUGt6ejlpVXc) をダウンロードしてインストールします。

辞書は `UTF-8` しか動作確認していません。

PostgreSQL 10.5 Windows x86-32 の場合:

- `C:\Program Files (x86)\MeCab\bin\libmecab.dll` を、`C:\Program Files (x86)\PostgreSQL\10\bin` へコピー
- `textsearch_ja-9.0.0-postgresql-10.0-x86` フォルダーの中身を `C:\Program Files (x86)\PostgreSQL\10` へコピー

PostgreSQL 10.5 Windows x86-64 の場合:

- [cmecab-java](https://code.google.com/archive/p/cmecab-java/downloads) へ向かい、`mecab-0.996_x64.zip` をダウンロードして展開
- `mecab/bin/libmecab.dll` を、`C:\Program Files\PostgreSQL\10\bin` へコピー
- `textsearch_ja-9.0.0-postgresql-10.0-x64` フォルダーの中身を `C:\Program Files (x86)\PostgreSQL\10` へコピー

続き:

textsearch_ja は、データベースごとにインストールできます。

対象のデータベースで SQL 文を実行:

```sql
CREATE EXTENSION textsearch_ja;
```

いまくいけば、これで完了です。

## 動作確認

```sql
check_textsearch_ja=# SELECT ja_wakachi('分かち書きを行います。');
         ja_wakachi
----------------------------
 分かち書き を 行い ます 。
(1 行)
```

```sql
SELECT furigana('漢字の読みをカタカナで返します。');
               furigana
--------------------------------------
 カンジノヨミヲカタカナデカエシマス。
(1 行)
```

```sql
SELECT * FROM ts_debug('japanese', E'日\n本\n語\n文\n字\n中\nの\n改\n行\nは\n除\n去\n');
 alias |    description    | token  |  dictionaries   |  dictionary   | lexemes
-------+-------------------+--------+-----------------+---------------+----------
 word  | Word, all letters | 日本語 | {japanese_stem} | japanese_stem | {日本語}
 word  | Word, all letters | 文字   | {japanese_stem} | japanese_stem | {文字}
 word  | Word, all letters | 中     | {japanese_stem} | japanese_stem | {中}
 blank | Space symbols     | の     | {}              |               |
 word  | Word, all letters | 改行   | {japanese_stem} | japanese_stem | {改行}
 blank | Space symbols     | は     | {}              |               |
 word  | Word, all letters | 除去   | {japanese_stem} | japanese_stem | {除去}
(7 行)
```

```sql
SELECT * FROM ts_debug('japanese', E'Line\nbreaks\nin\nEnglish\ntext\nare\nreserved.');
   alias   |   description   |  token   |  dictionaries  |  dictionary  |  lexemes
-----------+-----------------+----------+----------------+--------------+-----------
 asciiword | Word, all ASCII | Line     | {english_stem} | english_stem | {line}
 blank     | Space symbols   |          | {}             |              |
 asciiword | Word, all ASCII | breaks   | {english_stem} | english_stem | {break}
 blank     | Space symbols   |          | {}             |              |
 asciiword | Word, all ASCII | in       | {english_stem} | english_stem | {}
 blank     | Space symbols   |          | {}             |              |
 asciiword | Word, all ASCII | English  | {english_stem} | english_stem | {english}
 blank     | Space symbols   |          | {}             |              |
 asciiword | Word, all ASCII | text     | {english_stem} | english_stem | {text}
 blank     | Space symbols   |          | {}             |              |
 asciiword | Word, all ASCII | are      | {english_stem} | english_stem | {}
 blank     | Space symbols   |          | {}             |              |
 asciiword | Word, all ASCII | reserved | {english_stem} | english_stem | {reserv}
 blank     | Space symbols   | .        | {}             |              |
(14 行)
```

```sql
 SELECT * FROM ts_debug('japanese', '日本語とEnglishがmixedな文も解析OKです。');
   alias   |    description    |  token  |  dictionaries   |  dictionary   |  lexemes
-----------+-------------------+---------+-----------------+---------------+-----------
 word      | Word, all letters | 日本語  | {japanese_stem} | japanese_stem | {日本語}
 blank     | Space symbols     | と      | {}              |               |
 asciiword | Word, all ASCII   | English | {english_stem}  | english_stem  | {english}
 blank     | Space symbols     | が      | {}              |               |
 asciiword | Word, all ASCII   | mixed   | {english_stem}  | english_stem  | {mix}
 blank     | Space symbols     | な      | {}              |               |
 word      | Word, all letters | 文      | {japanese_stem} | japanese_stem | {文}
 blank     | Space symbols     | も      | {}              |               |
 word      | Word, all letters | 解析    | {japanese_stem} | japanese_stem | {解析}
 asciiword | Word, all ASCII   | OK      | {english_stem}  | english_stem  | {ok}
 blank     | Space symbols     | です    | {}              |               |
 blank     | Space symbols     | 。      | {}              |               |
(12 行)
```

```sql
SELECT s
  FROM regexp_split_to_table(to_tsvector('japanese',
check_textsearch_ja(# '語尾は基本形に戻されます。')::text, ' ') AS t(s)
  ORDER BY s;
     s
------------
 'れる':4
 '基本形':2
 '戻す':3
 '語尾':1
(4 行)
```

```sql
SELECT s
   FROM regexp_split_to_table(to_tsvector('japanese',
 'ユーザとユーザーは正規化されます。ミラーとミラは別扱い。')::text, ' ') AS t(s)
  ORDER BY s;
      s
--------------
 'する':5
 'れる':6
 'ミラ':8
 'ミラー':7
 'ユーザ':1,2
 '別':9
 '化':4
 '扱い':10
 '正規':3
(9 行)
```
