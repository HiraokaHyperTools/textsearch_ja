\set ECHO all
SET client_encoding = utf8;

SELECT ja_wakachi('分かち書きを行います。');

SELECT furigana('漢字の読みをカタカナで返します。');

SELECT * FROM ts_debug('japanese', E'日\n本\n語\n文\n字\n中\nの\n改\n行\nは\n除\n去\n');
SELECT * FROM ts_debug('japanese', E'Line\nbreaks\nin\nEnglish\ntext\nare\nreserved.');
SELECT * FROM ts_debug('japanese', '日本語とEnglishがmixedな文も解析OKです。');

SELECT s
  FROM regexp_split_to_table(to_tsvector('japanese',
		'語尾は基本形に戻されます。')::text, ' ') AS t(s)
 ORDER BY s;

SELECT s
  FROM regexp_split_to_table(to_tsvector('japanese',
		'ユーザとユーザーは正規化されます。ミラーとミラは別扱い。')::text, ' ') AS t(s)
 ORDER BY s;
