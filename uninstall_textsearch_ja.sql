SET search_path = public;

DROP TEXT SEARCH CONFIGURATION pg_catalog.japanese CASCADE;
DROP TEXT SEARCH PARSER pg_catalog.japanese CASCADE;
DROP TEXT SEARCH DICTIONARY japanese_stem CASCADE;
DROP TEXT SEARCH TEMPLATE pg_catalog.mecab CASCADE;

DROP FUNCTION ts_ja_start(internal, int4);
DROP FUNCTION ts_ja_gettoken(internal, internal, internal);
DROP FUNCTION ts_ja_end(internal);
DROP FUNCTION ts_ja_lexize(internal, internal, internal, internal);
DROP FUNCTION ja_analyze(text);
DROP FUNCTION ja_normalize(text);
DROP FUNCTION ja_wakachi(text);
DROP FUNCTION web_query(text);
DROP FUNCTION furigana(text);
DROP FUNCTION hiragana(text);
DROP FUNCTION katakana(text);
