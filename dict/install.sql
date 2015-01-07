\echo NOTICE: Copy *.syn files into $PGDATA/shared/tsearch_data before install.

BEGIN;
-- Install japanese synonym dictionary
CREATE TEXT SEARCH DICTIONARY japanese_syn (
    TEMPLATE = synonym,
    SYNONYMS = japanese
);
ALTER TEXT SEARCH CONFIGURATION japanese
    ALTER MAPPING FOR word, hword_part, hword WITH japanese_syn, simple;

-- Install english synonym dictionary
CREATE TEXT SEARCH DICTIONARY english_syn (
    TEMPLATE = synonym,
    SYNONYMS = english
);
ALTER TEXT SEARCH CONFIGURATION japanese
    ALTER MAPPING FOR asciiword, hword_asciipart, asciihword WITH english_syn, english_stem;
COMMIT;
