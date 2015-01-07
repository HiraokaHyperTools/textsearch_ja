\set ECHO all
SET client_encoding = utf8;

SELECT ja_normalize('０１２３４５６７８９0123456789');
SELECT ja_normalize('abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ');
SELECT ja_normalize('ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ');
SELECT ja_normalize('あいうえおアイウエオ');
SELECT ja_normalize(E'!"#$%&''()-=^~\\|@`[{;+:*]},<.>/?_');
SELECT ja_normalize('！”＃＄％＆’（）－＝＾￣￥｜＠‘［｛；＋：＊］｝，＜．＞／？＿');
SELECT ja_normalize('ｰｧｱｨｲｩｳｪｴｫｵｶｷｸｹｺｻｼｽｾｿﾀﾁｯﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓｬﾔｭﾕｮﾖﾗﾘﾙﾚﾛﾜｦﾝ');
SELECT ja_normalize('ｶﾞｷﾞｸﾞｹﾞｺﾞｻﾞｼﾞｽﾞｾﾞｿﾞﾀﾞﾁﾞﾂﾞﾃﾞﾄﾞﾊﾞﾊﾟﾋﾞﾋﾟﾌﾞﾌﾟﾍﾞﾍﾟﾎﾞﾎﾟｳﾞ');
SELECT ja_normalize('ｶ゛ｷ゛ｸ゛ｹ゛ｺ゛ｻ゛ｼ゛ｽ゛ｾ゛ｿ゛ﾀ゛ﾁ゛ﾂ゛ﾃ゛ﾄ゛ﾊ゛ﾊ゜ﾋ゛ﾋ゜ﾌ゛ﾌ゜ﾍ゛ﾍ゜ﾎ゛ﾎ゜ｳ゛');
SELECT ja_normalize('カﾞキﾞクﾞケﾞコﾞサﾞシﾞスﾞセﾞソﾞタﾞチﾞツﾞテﾞトﾞハﾞハﾟヒﾞヒﾟフﾞフﾟヘﾞヘﾟホﾞホﾟウﾞ');
SELECT ja_normalize('カ゛キ゛ク゛ケ゛コ゛サ゛シ゛ス゛セ゛ソ゛タ゛チ゛ツ゛テ゛ト゛ハ゛ハ゜ヒ゛ヒ゜フ゛フ゜ヘ゛ヘ゜ホ゛ホ゜ウ゛');
SELECT ja_normalize('かﾞきﾞくﾞけﾞこﾞさﾞしﾞすﾞせﾞそﾞたﾞちﾞつﾞてﾞとﾞはﾞはﾟひﾞひﾟふﾞふﾟへﾞへﾟほﾞほﾟうﾞ');
SELECT ja_normalize('か゛き゛く゛け゛こ゛さ゛し゛す゛せ゛そ゛た゛ち゛つ゛て゛と゛は゛は゜ひ゛ひ゜ふ゛ふ゜へ゛へ゜ほ゛ほ゜う゛');

SELECT hiragana('あいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほ');
SELECT hiragana('まみむめもやゐゆゑよらりるれろわをんぁぃぅぇぉっゃゅょゎ');
SELECT hiragana('がぎぐげござじずぜぞだぢづでどばびぶべぼぱぴぷぺぽ');
SELECT hiragana('アイウエオカキクケコサシスセソタチツテトナニヌネノハヒフヘホ');
SELECT hiragana('マミムメモヤヰユヱヨラリルレロワヲンァィゥェォッャュョヮ');
SELECT hiragana('ガギグゲゴザジズゼゾダヂヅデドバビブベボパピプペポヴ');
SELECT hiragana('ｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎ');
SELECT hiragana('ﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜｦﾝｧｨｩｪｫｯｬｭｮ');
SELECT hiragana('ｶﾞｷﾞｸﾞｹﾞｺﾞｻﾞｼﾞｽﾞｾﾞｿﾞﾀﾞﾁﾞﾂﾞﾃﾞﾄﾞﾊﾞﾋﾞﾌﾞﾍﾞﾎﾞﾊﾟﾋﾟﾌﾟﾍﾟﾎﾟｳﾞ');

SELECT katakana('あいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほ');
SELECT katakana('まみむめもやゐゆゑよらりるれろわをんぁぃぅぇぉっゃゅょゎ');
SELECT katakana('がぎぐげござじずぜぞだぢづでどばびぶべぼぱぴぷぺぽ');
SELECT katakana('アイウエオカキクケコサシスセソタチツテトナニヌネノハヒフヘホ');
SELECT katakana('マミムメモヤヰユヱヨラリルレロワヲンァィゥェォッャュョヮ');
SELECT katakana('ガギグゲゴザジズゼゾダヂヅデドバビブベボパピプペポヴ');
