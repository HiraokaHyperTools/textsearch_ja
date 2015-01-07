/*
 * normalize_eucjp.c
 */
#include "postgres.h"
#include "mb/pg_wchar.h"

#include "textsearch_ja.h"

#define WIDEN(c1, c2)		(((unsigned char) (c1) << 8) | (unsigned char) (c2))
#define BETWEEN(c, lo, hi)	((lo) <= ((unsigned char) (c)) && ((unsigned char) (c)) <= (hi))

/* アルファベットと数字の変換 */
#define EUCJP_IS_ALNUM(s) \
	(s[0] == 0xa3 && \
     (BETWEEN(s[1], 0xb0, 0xb9) || BETWEEN(s[1], 0xc1, 0xda) || BETWEEN(s[1], 0xe1, 0xfa)))
#define EUCJP_TO_ALNUM(s)	((char) (s[1] - 0x80))

typedef struct Map
{
	unsigned char	from;
	unsigned char	to[3];
} Map;

/* 記号の変換 */
static const Map EUCJP_SYMBOL[] =
{
	{ 0xa1, { 0x20 } },
	{ 0xa4, { 0x2c } },
	{ 0xa5, { 0x2e } },
	{ 0xa7, { 0x3a } },
	{ 0xa8, { 0x3b } },
	{ 0xa9, { 0x3f } },
	{ 0xaa, { 0x21 } },
	{ 0xb0, { 0x5e } },
	{ 0xb1, { 0x7e } },
	{ 0xb2, { 0x5f } },
	{ 0xbf, { 0x2f } },
	{ 0xc3, { 0x7c } },
	{ 0xc6, { 0x60 } },
	{ 0xc7, { 0x27 } },
	{ 0xc9, { 0x22 } },
	{ 0xca, { 0x28 } },
	{ 0xcb, { 0x29 } },
	{ 0xce, { 0x5b } },
	{ 0xcf, { 0x5d } },
	{ 0xd0, { 0x7b } },
	{ 0xd1, { 0x7d } },
	{ 0xdc, { 0x2b } },
	{ 0xdd, { 0x2d } },
	{ 0xe1, { 0x3d } },
	{ 0xe3, { 0x3c } },
	{ 0xe4, { 0x3e } },
	{ 0xef, { 0x5c } },
	{ 0xf0, { 0x24 } },
	{ 0xf3, { 0x25 } },
	{ 0xf4, { 0x23 } },
	{ 0xf5, { 0x26 } },
	{ 0xf6, { 0x2a } },
	{ 0xf7, { 0x40 } },
};

/* 半角カタカナの変換 */
static const Map EUCJP_KANA[] =
{
	{ 0xa6, { 0xa5, 0xf2 } },
	{ 0xa7, { 0xa5, 0xa1 } },
	{ 0xa8, { 0xa5, 0xa3 } },
	{ 0xa9, { 0xa5, 0xa5 } },
	{ 0xaa, { 0xa5, 0xa7 } },
	{ 0xab, { 0xa5, 0xa9 } },
	{ 0xac, { 0xa5, 0xe3 } },
	{ 0xad, { 0xa5, 0xe5 } },
	{ 0xae, { 0xa5, 0xe7 } },
	{ 0xaf, { 0xa5, 0xc3 } },
	{ 0xb0, { 0xa1, 0xbc } },
	{ 0xb1, { 0xa5, 0xa2 } },
	{ 0xb2, { 0xa5, 0xa4 } },
	{ 0xb3, { 0xa5, 0xa6 } },
	{ 0xb4, { 0xa5, 0xa8 } },
	{ 0xb5, { 0xa5, 0xaa } },
	{ 0xb6, { 0xa5, 0xab } },
	{ 0xb7, { 0xa5, 0xad } },
	{ 0xb8, { 0xa5, 0xaf } },
	{ 0xb9, { 0xa5, 0xb1 } },
	{ 0xba, { 0xa5, 0xb3 } },
	{ 0xbb, { 0xa5, 0xb5 } },
	{ 0xbc, { 0xa5, 0xb7 } },
	{ 0xbd, { 0xa5, 0xb9 } },
	{ 0xbe, { 0xa5, 0xbb } },
	{ 0xbf, { 0xa5, 0xbd } },
	{ 0xc0, { 0xa5, 0xbf } },
	{ 0xc1, { 0xa5, 0xc1 } },
	{ 0xc2, { 0xa5, 0xc4 } },
	{ 0xc3, { 0xa5, 0xc6 } },
	{ 0xc4, { 0xa5, 0xc8 } },
	{ 0xc5, { 0xa5, 0xca } },
	{ 0xc6, { 0xa5, 0xcb } },
	{ 0xc7, { 0xa5, 0xcc } },
	{ 0xc8, { 0xa5, 0xcd } },
	{ 0xc9, { 0xa5, 0xce } },
	{ 0xca, { 0xa5, 0xcf } },
	{ 0xcb, { 0xa5, 0xd2 } },
	{ 0xcc, { 0xa5, 0xd5 } },
	{ 0xcd, { 0xa5, 0xd8 } },
	{ 0xce, { 0xa5, 0xdb } },
	{ 0xcf, { 0xa5, 0xde } },
	{ 0xd0, { 0xa5, 0xdf } },
	{ 0xd1, { 0xa5, 0xe0 } },
	{ 0xd2, { 0xa5, 0xe1 } },
	{ 0xd3, { 0xa5, 0xe2 } },
	{ 0xd4, { 0xa5, 0xe4 } },
	{ 0xd5, { 0xa5, 0xe6 } },
	{ 0xd6, { 0xa5, 0xe8 } },
	{ 0xd7, { 0xa5, 0xe9 } },
	{ 0xd8, { 0xa5, 0xea } },
	{ 0xd9, { 0xa5, 0xeb } },
	{ 0xda, { 0xa5, 0xec } },
	{ 0xdb, { 0xa5, 0xed } },
	{ 0xdc, { 0xa5, 0xef } },
	{ 0xdd, { 0xa5, 0xf3 } },
	{ 0xde, { 0xa1, 0xab } },
	{ 0xdf, { 0xa1, 0xac } },
};

/* 半角カタカナの変換 */
static const Map EUCJP_HANKANA2ZENHIRA[] =
{
	{ 0xa6, { 0xa4, 0xf2 } },
	{ 0xa7, { 0xa4, 0xa1 } },
	{ 0xa8, { 0xa4, 0xa3 } },
	{ 0xa9, { 0xa4, 0xa5 } },
	{ 0xaa, { 0xa4, 0xa7 } },
	{ 0xab, { 0xa4, 0xa9 } },
	{ 0xac, { 0xa4, 0xe3 } },
	{ 0xad, { 0xa4, 0xe5 } },
	{ 0xae, { 0xa4, 0xe7 } },
	{ 0xaf, { 0xa4, 0xc3 } },
	{ 0xb1, { 0xa4, 0xa2 } },
	{ 0xb2, { 0xa4, 0xa4 } },
	{ 0xb3, { 0xa4, 0xa6 } },
	{ 0xb4, { 0xa4, 0xa8 } },
	{ 0xb5, { 0xa4, 0xaa } },
	{ 0xb6, { 0xa4, 0xab } },
	{ 0xb7, { 0xa4, 0xad } },
	{ 0xb8, { 0xa4, 0xaf } },
	{ 0xb9, { 0xa4, 0xb1 } },
	{ 0xba, { 0xa4, 0xb3 } },
	{ 0xbb, { 0xa4, 0xb5 } },
	{ 0xbc, { 0xa4, 0xb7 } },
	{ 0xbd, { 0xa4, 0xb9 } },
	{ 0xbe, { 0xa4, 0xbb } },
	{ 0xbf, { 0xa4, 0xbd } },
	{ 0xc0, { 0xa4, 0xbf } },
	{ 0xc1, { 0xa4, 0xc1 } },
	{ 0xc2, { 0xa4, 0xc4 } },
	{ 0xc3, { 0xa4, 0xc6 } },
	{ 0xc4, { 0xa4, 0xc8 } },
	{ 0xc5, { 0xa4, 0xca } },
	{ 0xc6, { 0xa4, 0xcb } },
	{ 0xc7, { 0xa4, 0xcc } },
	{ 0xc8, { 0xa4, 0xcd } },
	{ 0xc9, { 0xa4, 0xce } },
	{ 0xca, { 0xa4, 0xcf } },
	{ 0xcb, { 0xa4, 0xd2 } },
	{ 0xcc, { 0xa4, 0xd5 } },
	{ 0xcd, { 0xa4, 0xd8 } },
	{ 0xce, { 0xa4, 0xdb } },
	{ 0xcf, { 0xa4, 0xde } },
	{ 0xd0, { 0xa4, 0xdf } },
	{ 0xd1, { 0xa4, 0xe0 } },
	{ 0xd2, { 0xa4, 0xe1 } },
	{ 0xd3, { 0xa4, 0xe2 } },
	{ 0xd4, { 0xa4, 0xe4 } },
	{ 0xd5, { 0xa4, 0xe6 } },
	{ 0xd6, { 0xa4, 0xe8 } },
	{ 0xd7, { 0xa4, 0xe9 } },
	{ 0xd8, { 0xa4, 0xea } },
	{ 0xd9, { 0xa4, 0xeb } },
	{ 0xda, { 0xa4, 0xec } },
	{ 0xdb, { 0xa4, 0xed } },
	{ 0xdc, { 0xa4, 0xef } },
	{ 0xdd, { 0xa4, 0xf3 } },
};

static const unsigned char DAKUTEN_HALF[2] = { 0x8e, 0xde };
static const unsigned char DAKUTEN_WIDE[2] = { 0xa1, 0xab };
static const unsigned char HANDAKU_HALF[2] = { 0x8e, 0xdf };
static const unsigned char HANDAKU_WIDE[2] = { 0xa1, 0xac };

static int
mapcmp(const void *lhs, const void *rhs)
{
	const Map			   *lc = lhs;
	const unsigned char	   *rc = rhs;

	if (lc->from < *rc)
		return -1;
	else if (lc->from > *rc)
		return +1;
	else
		return 0;
}

static void
appendMappedChar(StringInfo dst, const unsigned char *src, size_t srclen, const Map *map, size_t maplen, append_t append)
{
	const Map *m;

	Assert(srclen >= 2);

	m = bsearch(&src[1], map, maplen, sizeof(Map), mapcmp);
	if (m != NULL)
		append(dst, m->to, uchar_strlen(m->to));
	else /* 変換対象文字ではなかったので、そのまま連結する. */
		append(dst, src, srclen);
}

#define EQ2(a, b)	(a[0] == b[0] && a[1] == b[1])

void
normalize_eucjp(StringInfo dst, const char *src, size_t srclen, append_t append)
{
	int		len;
	const unsigned char *s = (const unsigned char *)src;
	const unsigned char *end = s + srclen;

	for (; s < end; s += len)
	{
		len = uchar_mblen(s);

		if (len == 1)
		{
			append(dst, s, len);
			continue;
		}

		/* 濁点、半濁点の処理 */
		if (len == 2 && dst->len >= 2 &&
			(*StringInfoTail(dst, 2) == 0xa4 || *StringInfoTail(dst, 2) == 0xa5))
		{
			if (EQ2(s, DAKUTEN_HALF) || EQ2(s, DAKUTEN_WIDE))
			{
				/* 濁点 */
				unsigned char *prev = StringInfoTail(dst, 2);
				switch (prev[1])
				{
				case 0xa6: /* ウ */
					prev[0] = 0xa5;
					prev[1] = 0xf4;
					continue;
				case 0xab: /* カ */
				case 0xad: /* キ */
				case 0xaf: /* ク */
				case 0xb1: /* ケ */
				case 0xb3: /* コ */
				case 0xb5: /* サ */
				case 0xb7: /* シ */
				case 0xb9: /* ス */
				case 0xbb: /* セ */
				case 0xbd: /* ソ */
				case 0xbf: /* タ */
				case 0xc1: /* チ */
				case 0xc4: /* ツ */
				case 0xc6: /* テ */
				case 0xc8: /* ト */
				case 0xcf: /* ハ */
				case 0xd2: /* ヒ */
				case 0xd5: /* フ */
				case 0xd8: /* ヘ */
				case 0xdb: /* ホ */
					prev[1] += 1;
					continue;
				}
			}
			else if (EQ2(s, HANDAKU_HALF) || EQ2(s, HANDAKU_WIDE))
			{
				/* 半濁点 */
				unsigned char *prev = StringInfoTail(dst, 2);
				switch (prev[1])
				{
				case 0xcf: /* ハ */
				case 0xd2: /* ヒ */
				case 0xd5: /* フ */
				case 0xd8: /* ヘ */
				case 0xdb: /* ホ */
					prev[1] += 2;
					continue;
				}
			}
		}

		if (EUCJP_IS_ALNUM(s))
			appendStringInfoChar(dst, EUCJP_TO_ALNUM(s));
		else if (s[0] == 0xa1)
			appendMappedChar(dst, s, len, EUCJP_SYMBOL, lengthof(EUCJP_SYMBOL), append);
		else if (s[0] == 0x8e)
			appendMappedChar(dst, s, len, EUCJP_KANA, lengthof(EUCJP_KANA), append);
		else
			append(dst, s, len);
	}
}

static const unsigned char DASH[] = { 0xa1, 0xbc };
#define CHAR_LEN	2
#define DASH_LEN	2

char *
lexize_eucjp(const char *s, size_t len)
{
	const unsigned char *str = (const unsigned char *) s;
	char *r;

	/* 1文字のひらがな, カタカナは削る. */
	if (len == CHAR_LEN && str[0] == 0xe3)
	{
		int	widen = WIDEN(str[1], str[2]);
		if (0x8181 <= widen && widen <= 0x83b6)
			return NULL;
	}

	/* 4文字以上で末尾が「ー」の場合は削る. */
	if (len >= (4 * CHAR_LEN) &&
		memcmp(&str[len - DASH_LEN], DASH, DASH_LEN) == 0)
		len -= DASH_LEN;

	r = palloc(len + 1);
	memcpy(r, str, len);
	r[len] = '\0';

	return r;
}

/*
 * 無視する単語の種類
 */

static const char JOSHI[] = { 0275, 0365, 0273, 0354, ',' };
static const char JODOU[] = { 0275, 0365, 0306, 0260, 0273, 0354, ',' };
static const char KIGOU[] = { 0265, 0255, 0271, 0346, ',' };
static const char BYWORD[] = { 0314, 0276, 0273, 0354, ',', 0302, 0345, 0314, 0276, 0273, 0354, ',' }; 
static const char INSUFF[] = { 0314, 0276, 0273, 0354, ',', 0310, 0363, 0274, 0253, 0316, 0251, ',' }; 
static const char KANDO[] = { 0264, 0266, 0306, 0260, 0273, 0354, ',' }; 
static const char FILLER[] = { 0245, 0325, 0245, 0243, 0245, 0351, 0241, 0274, ',' }; 
static const char OTHERS[] = { 0244, 0275, 0244, 0316, 0302, 0276, ',' };

const IgnorableWord	IGNORE_EUCJP[] =
{
	{ lengthof(JOSHI), JOSHI },		/* 助詞 */
	{ lengthof(JODOU), JODOU },		/* 助動詞 */
	{ lengthof(KIGOU), KIGOU },		/* 記号 */
	{ lengthof(BYWORD), BYWORD },	/* 名詞,代名詞 */
	{ lengthof(INSUFF), INSUFF },	/* 名詞,非自立 */
	{ lengthof(KANDO), KANDO },		/* 感動詞 */
	{ lengthof(FILLER), FILLER },	/* フィラー */
	{ lengthof(OTHERS), OTHERS },	/* その他 */
	{ 0 }
};

/*
 * カタカナ -> ひらがな
 */
void
hiragana_eucjp(StringInfo dst, const char *src, size_t srclen)
{
	int		len;
	const unsigned char *s = (const unsigned char *)src;
	const unsigned char *end = s + srclen;

	for (; s < end; s += len)
	{
		len = uchar_mblen(s);

		if (len != 2)
		{
			appendMBString(dst, s, len);
			continue;
		}

		/* 濁点、半濁点の処理 */
		if (dst->len >= 2 && *StringInfoTail(dst, 2) == 0xa4)
		{
			if (EQ2(s, DAKUTEN_HALF) || EQ2(s, DAKUTEN_WIDE))
			{
				/* 濁点 */
				unsigned char *prev = StringInfoTail(dst, 2);
				switch (prev[1])
				{
				case 0xab: /* か */
				case 0xad: /* き */
				case 0xaf: /* く */
				case 0xb1: /* け */
				case 0xb3: /* こ */
				case 0xb5: /* さ */
				case 0xb7: /* し */
				case 0xb9: /* す */
				case 0xbb: /* せ */
				case 0xbd: /* そ */
				case 0xbf: /* た */
				case 0xc1: /* ち */
				case 0xc4: /* つ */
				case 0xc6: /* て */
				case 0xc8: /* と */
				case 0xcf: /* は */
				case 0xd2: /* ひ */
				case 0xd5: /* ふ */
				case 0xd8: /* へ */
				case 0xdb: /* ほ */
					prev[1] += 1;
					continue;
				}
			}
			else if (EQ2(s, HANDAKU_HALF) || EQ2(s, HANDAKU_WIDE))
			{
				/* 半濁点 */
				unsigned char *prev = StringInfoTail(dst, 2);
				switch (prev[1])
				{
				case 0xcf: /* は */
				case 0xd2: /* ひ */
				case 0xd5: /* ふ */
				case 0xd8: /* へ */
				case 0xdb: /* ほ */
					prev[1] += 2;
					continue;
				}
			}
		}

		if (s[0] == 0xa5)
		{
			if (0xa1 <= s[1] && s[1] <= 0xf3)
			{
				appendStringInfoChar(dst, 0xa4);
				appendStringInfoChar(dst, s[1]);
				continue;
			}
		}
		else if (s[0] == 0x8e)
		{
			appendMappedChar(dst, s, len, EUCJP_HANKANA2ZENHIRA,
				lengthof(EUCJP_HANKANA2ZENHIRA), (append_t) appendBinaryStringInfo);
			continue;
		}

		appendMBString(dst, s, len);
	}
}

/*
 * ひらがな -> カタカナ
 */
void
katakana_eucjp(StringInfo dst, const char *src, size_t srclen)
{
	int		len;
	const unsigned char *s = (const unsigned char *)src;
	const unsigned char *end = s + srclen;

	for (; s < end; s += len)
	{
		len = uchar_mblen(s);

		if (len == 2 && s[0] == 0xa4 && 0xa1 <= s[1] && s[1] <= 0xf3)
		{
			appendStringInfoChar(dst, 0xa5);
			appendStringInfoChar(dst, s[1]);
			continue;
		}

		appendMBString(dst, s, len);
	}
}
