/*
 * CDDL HEADER START
 *
 * This file and its contents are supplied under the terms of the
 * Common Development and Distribution License ("CDDL"), version 1.0.
 * You may only use this file in accordance with the terms of version
 * 1.0 of the CDDL.
 *
 * A full copy of the text of the CDDL should have accompanied this
 * source.  A copy of the CDDL is also available via the Internet at
 * http://www.illumos.org/license/CDDL.
 *
 * CDDL HEADER END
*/
/*
 * Copyright 2017 Saso Kiselkov. All rights reserved.
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <acfutils/icao2cc.h>

typedef struct {
	const char *icao;
	const char *cc;
	const char *lang;
} icao2cc_t;

/*
 * Although we'd love to have this table be uniquely keyed by ICAO code,
 * unfortunately not every place has its own unique ICAO prefix. However,
 * they're usually small enough that we don't need to worry too much.
 * Not all ICAO codes are strictly prefix-based. So to avoid matching a
 * more general (shorter) code before a more specific (longer) one, we lay
 * this table out to place more specific codes ahead of the more general
 * ones.
 */
static icao2cc_t icao2cc_table[] = {
    { .icao = "AG", .cc = "SB", .lang = NULL },	/* Solomon Islands */
    { .icao = "AN", .cc = "NR", .lang = NULL },	/* Nauru */
    { .icao = "AY", .cc = "PG", .lang = NULL },	/* Papua New Guinea */
    { .icao = "BG", .cc = "GL", .lang = NULL },	/* Greenland */
    { .icao = "BI", .cc = "IS", .lang = "is" },	/* Iceland */
    { .icao = "BK", .cc = "XK", .lang = NULL },	/* Kosovo */
    { .icao = "C", .cc = "CA", .lang = "en" },	/* Canada */
    { .icao = "DA", .cc = "DZ", .lang = "ar" },	/* Algeria */
    { .icao = "DB", .cc = "BJ", .lang = NULL },	/* Benin */
    { .icao = "DF", .cc = "BF", .lang = NULL },	/* Burkina Faso */
    { .icao = "DG", .cc = "GH", .lang = NULL },	/* Ghana */
    { .icao = "DI", .cc = "CI", .lang = NULL },	/* Ivory Coast */
    { .icao = "DN", .cc = "NG", .lang = NULL },	/* Nigeria */
    { .icao = "DR", .cc = "NE", .lang = NULL },	/* Niger */
    { .icao = "DT", .cc = "TN", .lang = NULL },	/* Tunisia */
    { .icao = "DX", .cc = "TG", .lang = NULL },	/* Togo */
    { .icao = "EB", .cc = "BE", .lang = "fr" },	/* Belgium */
    { .icao = "ED", .cc = "DE", .lang = "de" },	/* Germany */
    { .icao = "EE", .cc = "EE", .lang = "et" },	/* Estonia */
    { .icao = "EF", .cc = "FI", .lang = "sv" },	/* Finland */
    { .icao = "EG", .cc = "GB", .lang = "en" },	/* United Kingdom */
    { .icao = "EG", .cc = "GS", .lang = NULL },	/* South Georgia and the */
						/* South Sandwich Islands */
    { .icao = "EH", .cc = "NL", .lang = "nl" },	/* Netherlands */
    { .icao = "EI", .cc = "IE", .lang = "en" },	/* Ireland */
    { .icao = "EK", .cc = "DK", .lang = "da" },	/* Denmark */
    { .icao = "EL", .cc = "LU", .lang = "de" },	/* Luxembourg */
    { .icao = "EN", .cc = "MH", .lang = NULL },	/* Marshall Islands */
    { .icao = "EN", .cc = "NO", .lang = "nn" },	/* Norway */
    { .icao = "EP", .cc = "PL", .lang = "pl" },	/* Poland */
    { .icao = "ES", .cc = "SE", .lang = "sv" },	/* Sweden */
    { .icao = "ET", .cc = "DE", .lang = "de" },	/* Germany */
    { .icao = "EV", .cc = "LV", .lang = "lv" },	/* Latvia */
    { .icao = "EY", .cc = "LT", .lang = "lt" },	/* Lithuania */
    { .icao = "FA", .cc = "ZA", .lang = "en" },	/* South Africa */
    { .icao = "FB", .cc = "BW", .lang = NULL },	/* Botswana */
    { .icao = "FC", .cc = "CG", .lang = NULL },	/* Republic of the Congo */
    { .icao = "FD", .cc = "SZ", .lang = NULL },	/* Swaziland */
    { .icao = "FE", .cc = "CF", .lang = NULL },	/* Central African Republic */
    { .icao = "FG", .cc = "GQ", .lang = NULL },	/* Equatorial Guinea */
    { .icao = "FH", .cc = "SH", .lang = NULL },	/* Saint Helena Ascension */
						/* and Tristan da Cunha */
    { .icao = "FI", .cc = "MU", .lang = NULL },	/* Mauritius */
    { .icao = "FJ", .cc = "IO", .lang = "en" },	/* British Indian Ocean */
						/* Territory */
    { .icao = "FK", .cc = "CM", .lang = NULL },	/* Cameroon */
    { .icao = "FL", .cc = "ZM", .lang = NULL },	/* Zambia */
    { .icao = "FMC", .cc = "KM", .lang = NULL },	/* Comoros */
    { .icao = "FME", .cc = "RE", .lang = NULL },	/* Réunion */
    { .icao = "FMM", .cc = "MG", .lang = NULL },	/* Madagascar */
    { .icao = "FMN", .cc = "MG", .lang = NULL },	/* Madagascar */
    { .icao = "FMS", .cc = "MG", .lang = NULL },	/* Madagascar */
    { .icao = "FM", .cc = "YT", .lang = NULL },	/* Mayotte */
    { .icao = "FN", .cc = "AO", .lang = NULL },	/* Angola */
    { .icao = "FO", .cc = "GA", .lang = NULL },	/* Gabon */
    { .icao = "FP", .cc = "ST", .lang = NULL },	/* São Tomé and Príncipe */
    { .icao = "FQ", .cc = "MZ", .lang = NULL },	/* Mozambique */
    { .icao = "FS", .cc = "SC", .lang = NULL },	/* Seychelles */
    { .icao = "FT", .cc = "TD", .lang = NULL },	/* Chad */
    { .icao = "FV", .cc = "ZW", .lang = "en" },	/* Zimbabwe */
    { .icao = "FW", .cc = "MW", .lang = NULL },	/* Malawi */
    { .icao = "FX", .cc = "LS", .lang = NULL },	/* Lesotho */
    { .icao = "FY", .cc = "NA", .lang = NULL },	/* Namibia */
    { .icao = "FZ", .cc = "CD", .lang = NULL },	/* Democratic Republic of */
						/* the Congo */
    { .icao = "GA", .cc = "ML", .lang = NULL },	/* Mali */
    { .icao = "GB", .cc = "GM", .lang = NULL },	/* Gambia */
    { .icao = "GC", .cc = "ES", .lang = "es" },	/* Spain */
    { .icao = "GE", .cc = "ES", .lang = "es" },	/* Spain */
    { .icao = "GF", .cc = "SL", .lang = NULL },	/* Sierra Leone */
    { .icao = "GG", .cc = "GW", .lang = NULL },	/* Guinea-Bissau */
    { .icao = "GL", .cc = "LR", .lang = NULL },	/* Liberia */
    { .icao = "GM", .cc = "MA", .lang = "ar" },	/* Morocco */
    { .icao = "GO", .cc = "SN", .lang = NULL },	/* Senegal */
    { .icao = "GQ", .cc = "MR", .lang = NULL },	/* Mauritania */
    { .icao = "GS", .cc = "EH", .lang = NULL },	/* Western Sahara */
    { .icao = "GU", .cc = "GN", .lang = NULL },	/* Guinea */
    { .icao = "GV", .cc = "CV", .lang = NULL },	/* Cape Verde */
    { .icao = "HA", .cc = "ET", .lang = NULL },	/* Ethiopia */
    { .icao = "HB", .cc = "BI", .lang = NULL },	/* Burundi */
    { .icao = "HC", .cc = "SO", .lang = NULL },	/* Somalia */
    { .icao = "HD", .cc = "DJ", .lang = NULL },	/* Djibouti */
    { .icao = "HE", .cc = "EG", .lang = "ar" },	/* Egypt */
    { .icao = "HH", .cc = "ER", .lang = NULL },	/* Eritrea */
    { .icao = "HK", .cc = "KE", .lang = "sw" },	/* Kenya */
    { .icao = "HL", .cc = "LY", .lang = "ar" },	/* Libya */
    { .icao = "HR", .cc = "RW", .lang = NULL },	/* Rwanda */
    { .icao = "HS", .cc = "SD", .lang = NULL },	/* Sudan */
    { .icao = "HS", .cc = "SS", .lang = NULL },	/* South Sudan */
    { .icao = "HT", .cc = "TZ", .lang = NULL },	/* Tanzania */
    { .icao = "HU", .cc = "UG", .lang = NULL },	/* Uganda */
    { .icao = "K", .cc = "US", .lang = "en" },	/* United States */
    { .icao = "LA", .cc = "AL", .lang = "sq" },	/* Albania */
    { .icao = "LB", .cc = "BG", .lang = NULL },	/* Bulgaria */
    { .icao = "LC", .cc = "CY", .lang = NULL },	/* Cyprus */
    { .icao = "LD", .cc = "HR", .lang = "hr" },	/* Croatia */
    { .icao = "LE", .cc = "ES", .lang = "es" },	/* Spain */
    { .icao = "LF", .cc = "FR", .lang = "fr" },	/* France */
    { .icao = "LF", .cc = "PM", .lang = NULL },	/* Saint Pierre and Miquelon */
    { .icao = "LG", .cc = "GR", .lang = "el" },	/* Greece */
    { .icao = "LH", .cc = "HU", .lang = "hu" },	/* Hungary */
    { .icao = "LI", .cc = "IT", .lang = "it" },	/* Italy */
    { .icao = "LJ", .cc = "SI", .lang = "sl" },	/* Slovenia */
    { .icao = "LK", .cc = "CZ", .lang = "cs" },	/* Czech Republic */
    { .icao = "LL", .cc = "IL", .lang = "he" },	/* Israel */
    { .icao = "LM", .cc = "MT", .lang = NULL },	/* Malta */
    { .icao = "LN", .cc = "MC", .lang = "fr" },	/* Monaco */
    { .icao = "LO", .cc = "AT", .lang = "de" },	/* Austria */
    { .icao = "LP", .cc = "PT", .lang = "pt" },	/* Portugal */
    { .icao = "LQ", .cc = "BA", .lang = NULL },	/* Bosnia and Herzegovina */
    { .icao = "LR", .cc = "RO", .lang = "ro" },	/* Romania */
    { .icao = "LS", .cc = "CH", .lang = "it" },	/* Switzerland */
    { .icao = "LT", .cc = "TR", .lang = "tr" },	/* Turkey */
    { .icao = "LU", .cc = "MD", .lang = NULL },	/* Moldova */
    { .icao = "LV", .cc = "PS", .lang = NULL },	/* Palestine */
    { .icao = "LW", .cc = "MK", .lang = "mk" },	/* Macedonia */
    { .icao = "LX", .cc = "GI", .lang = NULL },	/* Gibraltar */
    { .icao = "LY", .cc = "ME", .lang = NULL },	/* Montenegro */
    { .icao = "LY", .cc = "RS", .lang = NULL },	/* Serbia */
    { .icao = "LZ", .cc = "SK", .lang = "sk" },	/* Slovakia */
    { .icao = "MB", .cc = "TC", .lang = NULL },	/* Turks and Caicos Islands */
    { .icao = "MD", .cc = "DO", .lang = "es" },	/* Dominican Republic */
    { .icao = "MG", .cc = "GT", .lang = "es" },	/* Guatemala */
    { .icao = "MH", .cc = "HN", .lang = "es" },	/* Honduras */
    { .icao = "MI", .cc = "VI", .lang = "en" },	/* United States */
						/* Virgin Islands */
    { .icao = "MK", .cc = "JM", .lang = "en" },	/* Jamaica */
    { .icao = "MM", .cc = "MX", .lang = "es" },	/* Mexico */
    { .icao = "MN", .cc = "NI", .lang = "es" },	/* Nicaragua */
    { .icao = "MP", .cc = "PA", .lang = "es" },	/* Panama */
    { .icao = "MR", .cc = "CR", .lang = "es" },	/* Costa Rica */
    { .icao = "MS", .cc = "SV", .lang = "es" },	/* El Salvador */
    { .icao = "MT", .cc = "HT", .lang = NULL },	/* Haiti */
    { .icao = "MU", .cc = "CU", .lang = NULL },	/* Cuba */
    { .icao = "MW", .cc = "KY", .lang = NULL },	/* Cayman Islands */
    { .icao = "MY", .cc = "BS", .lang = NULL },	/* Bahamas */
    { .icao = "MZ", .cc = "BZ", .lang = "en" },	/* Belize */
    { .icao = "NC", .cc = "CK", .lang = NULL },	/* Cook Islands */
    { .icao = "NE", .cc = "CL", .lang = "es" },	/* Chile */
    { .icao = "NFT", .cc = "TO", .lang = NULL },	/* Tonga */
    { .icao = "NF", .cc = "FJ", .lang = NULL },	/* Fiji */
    { .icao = "NGF", .cc = "TV", .lang = NULL },	/* Tuvalu */
    { .icao = "NG", .cc = "KI", .lang = NULL },	/* Kiribati */
    { .icao = "NI", .cc = "NU", .lang = NULL },	/* Niue */
    { .icao = "NL", .cc = "WF", .lang = NULL },	/* Wallis and Futuna */
    { .icao = "NS", .cc = "AS", .lang = "en" },	/* American Samoa */
    { .icao = "NS", .cc = "WS", .lang = NULL },	/* Samoa */
    { .icao = "NT", .cc = "PF", .lang = NULL },	/* French Polynesia */
    { .icao = "NV", .cc = "VU", .lang = NULL },	/* Vanuatu */
    { .icao = "NW", .cc = "NC", .lang = NULL },	/* New Caledonia */
    { .icao = "NZ", .cc = "NZ", .lang = "en" },	/* New Zealand */
    { .icao = "OA", .cc = "AF", .lang = NULL },	/* Afghanistan */
    { .icao = "OB", .cc = "BH", .lang = "ar" },	/* Bahrain */
    { .icao = "OE", .cc = "SA", .lang = "ar" },	/* Saudi Arabia */
    { .icao = "OI", .cc = "IR", .lang = "fa" },	/* Iran */
    { .icao = "OJ", .cc = "JO", .lang = "ar" },	/* Jordan */
    { .icao = "OJ", .cc = "PS", .lang = NULL },	/* Palestine */
    { .icao = "OK", .cc = "KW", .lang = "ar" },	/* Kuwait */
    { .icao = "OL", .cc = "LB", .lang = "ar" },	/* Lebanon */
    { .icao = "OM", .cc = "AE", .lang = "ar" },	/* United Arab Emirates */
    { .icao = "OO", .cc = "OM", .lang = "ar" },	/* Oman */
    { .icao = "OP", .cc = "PK", .lang = "ur" },	/* Pakistan */
    { .icao = "OR", .cc = "IQ", .lang = "ar" },	/* Iraq */
    { .icao = "OS", .cc = "SY", .lang = "syr" },/* Syria */
    { .icao = "OT", .cc = "QA", .lang = "ar" },	/* Qatar */
    { .icao = "OY", .cc = "YE", .lang = NULL },	/* Yemen */
    { .icao = "PA", .cc = "US", .lang = "en" },	/* United States */
    { .icao = "PB", .cc = "US", .lang = "en" },	/* United States */
    { .icao = "PF", .cc = "US", .lang = "en" },	/* United States */
    { .icao = "PG", .cc = "GU", .lang = NULL },	/* Guam */
    { .icao = "PG", .cc = "MP", .lang = NULL },	/* Northern Mariana Islands */
    { .icao = "PH", .cc = "US", .lang = "en" },	/* United States */
    { .icao = "PJ", .cc = "US", .lang = "en" },	/* United States */
    { .icao = "PK", .cc = "MH", .lang = NULL },	/* Marshall Islands */
    { .icao = "PL", .cc = "NZ", .lang = "en" },	/* New Zealand */
    { .icao = "PL", .cc = "US", .lang = "en" },	/* United States */
    { .icao = "PM", .cc = "US", .lang = "en" },	/* United States */
    { .icao = "PO", .cc = "US", .lang = "en" },	/* United States */
    { .icao = "PP", .cc = "US", .lang = "en" },	/* United States */
    { .icao = "PT", .cc = "FM", .lang = NULL },	/* Federated States of Micronesia */
    { .icao = "PT", .cc = "PW", .lang = NULL },	/* Palau */
    { .icao = "PW", .cc = "US", .lang = "en" },	/* United States */
    { .icao = "RC", .cc = "TW", .lang = "zh" },	/* Taiwan */
    { .icao = "RJ", .cc = "JP", .lang = "ja" },	/* Japan */
    { .icao = "RK", .cc = "KR", .lang = "ko" },	/* South Korea */
    { .icao = "RO", .cc = "JP", .lang = "ja" },	/* Japan */
    { .icao = "RP", .cc = "PH", .lang = "en" },	/* Philippines */
    { .icao = "SA", .cc = "AR", .lang = "es" },	/* Argentina */
    { .icao = "SB", .cc = "BR", .lang = "pt" },	/* Brazil */
    { .icao = "SC", .cc = "CL", .lang = "es" },	/* Chile */
    { .icao = "SD", .cc = "BR", .lang = "pt" },	/* Brazil */
    { .icao = "SE", .cc = "EC", .lang = "es" },	/* Ecuador */
    { .icao = "SF", .cc = "FK", .lang = NULL },	/* Falkland Islands */
    { .icao = "SG", .cc = "PY", .lang = "es" },	/* Paraguay */
    { .icao = "SK", .cc = "CO", .lang = "es" },	/* Colombia */
    { .icao = "SL", .cc = "BO", .lang = "es" },	/* Bolivia */
    { .icao = "SM", .cc = "SR", .lang = NULL },	/* Suriname */
    { .icao = "SN", .cc = "BR", .lang = "pt" },	/* Brazil */
    { .icao = "SO", .cc = "GF", .lang = NULL },	/* French Guiana */
    { .icao = "SP", .cc = "PE", .lang = "es" },	/* Peru */
    { .icao = "SS", .cc = "BR", .lang = "pt" },	/* Brazil */
    { .icao = "SU", .cc = "UY", .lang = "es" },	/* Uruguay */
    { .icao = "SV", .cc = "VE", .lang = "es" },	/* Venezuela */
    { .icao = "SW", .cc = "BR", .lang = "pt" },	/* Brazil */
    { .icao = "SY", .cc = "GY", .lang = NULL },	/* Guyana */
    { .icao = "TA", .cc = "AG", .lang = NULL },	/* Antigua and Barbuda */
    { .icao = "TB", .cc = "BB", .lang = NULL },	/* Barbados */
    { .icao = "TD", .cc = "DM", .lang = NULL },	/* Dominica */
    { .icao = "TF", .cc = "BL", .lang = "fr" },	/* Saint Barthélemy */
    { .icao = "TF", .cc = "GP", .lang = "fr" },	/* Guadeloupe */
    { .icao = "TF", .cc = "MF", .lang = "fr" },	/* Saint Martin */
    { .icao = "TF", .cc = "MQ", .lang = "fr" },	/* Martinique */
    { .icao = "TG", .cc = "GD", .lang = "en" },	/* Grenada */
    { .icao = "TI", .cc = "VI", .lang = "en" },	/* United States */
						/* Virgin Islands */
    { .icao = "TJ", .cc = "PR", .lang = "es" },	/* Puerto Rico */
    { .icao = "TK", .cc = "KN", .lang = "en" },	/* Saint Kitts and Nevis */
    { .icao = "TL", .cc = "LC", .lang = NULL },	/* Saint Lucia */
    { .icao = "TN", .cc = "AW", .lang = "nl" },	/* Aruba */
    { .icao = "TN", .cc = "BQ", .lang = "nl" },	/* Caribbean Netherlands */
    { .icao = "TN", .cc = "CW", .lang = "nl" },	/* Curaçao */
    { .icao = "TN", .cc = "SX", .lang = "nl" },	/* Sint Maarten */
    { .icao = "TQ", .cc = "AI", .lang = NULL },	/* Anguilla */
    { .icao = "TR", .cc = "MS", .lang = NULL },	/* Montserrat */
    { .icao = "TT", .cc = "TT", .lang = "en" },	/* Trinidad and Tobago */
    { .icao = "TU", .cc = "VG", .lang = "en" },	/* British Virgin Islands */
    { .icao = "TV", .cc = "VC", .lang = NULL },	/* Saint Vincent and */
						/* the Grenadines */
    { .icao = "TX", .cc = "BM", .lang = NULL },	/* Bermuda */
    { .icao = "UA", .cc = "KZ", .lang = "ky" },	/* Kazakhstan */
    { .icao = "UB", .cc = "AZ", .lang = NULL },	/* Azerbaijan */
    { .icao = "UC", .cc = "KG", .lang = NULL },	/* Kyrgyzstan */
    { .icao = "UD", .cc = "AM", .lang = NULL },	/* Armenia */
    { .icao = "UE", .cc = "RU", .lang = "ru" },	/* Russia */
    { .icao = "UG", .cc = "GE", .lang = "ka" },	/* Georgia */
    { .icao = "UH", .cc = "RU", .lang = "ru" },	/* Russia */
    { .icao = "UI", .cc = "RU", .lang = "ru" },	/* Russia */
    { .icao = "UK", .cc = "UA", .lang = "uk" },	/* Ukraine */
    { .icao = "UL", .cc = "RU", .lang = "ru" },	/* Russia */
    { .icao = "UM", .cc = "BY", .lang = NULL },	/* Belarus */
    { .icao = "UN", .cc = "RU", .lang = "ru" },	/* Russia */
    { .icao = "UO", .cc = "RU", .lang = "ru" },	/* Russia */
    { .icao = "UR", .cc = "RU", .lang = "ru" },	/* Russia */
    { .icao = "US", .cc = "RU", .lang = "ru" },	/* Russia */
    { .icao = "UT", .cc = "TJ", .lang = NULL },	/* Tajikistan */
    { .icao = "UT", .cc = "TM", .lang = NULL },	/* Turkmenistan */
    { .icao = "UT", .cc = "UZ", .lang = "uz" },	/* Uzbekistan */
    { .icao = "UU", .cc = "RU", .lang = "ru" },	/* Russia */
    { .icao = "UW", .cc = "RU", .lang = "ru" },	/* Russia */
    { .icao = "VA", .cc = "IN", .lang = "hi" },	/* India */
    { .icao = "VB", .cc = "MM", .lang = NULL },	/* Myanmar */
    { .icao = "VC", .cc = "LK", .lang = NULL },	/* Sri Lanka */
    { .icao = "VD", .cc = "KH", .lang = NULL },	/* Cambodia */
    { .icao = "VE", .cc = "IN", .lang = "te" },	/* India */
    { .icao = "VG", .cc = "BD", .lang = NULL },	/* Bangladesh */
    { .icao = "VH", .cc = "HK", .lang = "zh" },	/* Hong Kong */
    { .icao = "VI", .cc = "IN", .lang = "te" },	/* India */
    { .icao = "VL", .cc = "LA", .lang = NULL },	/* Laos */
    { .icao = "VM", .cc = "MO", .lang = "zh" },	/* Macau */
    { .icao = "VN", .cc = "NP", .lang = NULL },	/* Nepal */
    { .icao = "VO", .cc = "IN", .lang = "te" },	/* India */
    { .icao = "VQ", .cc = "BT", .lang = NULL },	/* Bhutan */
    { .icao = "VR", .cc = "MV", .lang = "div" },/* Maldives */
    { .icao = "VT", .cc = "TH", .lang = "th" },	/* Thailand */
    { .icao = "VV", .cc = "VN", .lang = "vi" },	/* Vietnam */
    { .icao = "VY", .cc = "MM", .lang = NULL },	/* Myanmar */
    { .icao = "WA", .cc = "ID", .lang = "id" },	/* Indonesia */
    { .icao = "WB", .cc = "BN", .lang = "ms" },	/* Brunei */
    { .icao = "WB", .cc = "MY", .lang = "ms" },	/* Malaysia */
    { .icao = "WI", .cc = "ID", .lang = "id" },	/* Indonesia */
    { .icao = "WM", .cc = "MY", .lang = "ms" },	/* Malaysia */
    { .icao = "WP", .cc = "TL", .lang = NULL },	/* Timor-Leste */
    { .icao = "WS", .cc = "SG", .lang = "zh" },	/* Singapore */
    { .icao = "YP", .cc = "CX", .lang = NULL },	/* Christmas Island */
    { .icao = "Y", .cc = "AU", .lang = "en" },	/* Australia */
    { .icao = "ZB", .cc = "CN", .lang = "zh" },	/* China */
    { .icao = "ZG", .cc = "CN", .lang = "zh" },	/* China */
    { .icao = "ZH", .cc = "CN", .lang = "zh" },	/* China */
    { .icao = "ZJ", .cc = "CN", .lang = "zh" },	/* China */
    { .icao = "ZK", .cc = "KP", .lang = "ko" },	/* North Korea */
    { .icao = "ZL", .cc = "CN", .lang = "zh" },	/* China */
    { .icao = "ZM", .cc = "MN", .lang = "mn" },	/* Mongolia */
    { .icao = "ZP", .cc = "CN", .lang = "zh" },	/* China */
    { .icao = "ZS", .cc = "CN", .lang = "zh" },	/* China */
    { .icao = "ZT", .cc = "CN", .lang = "zh" },	/* China */
    { .icao = "ZU", .cc = "CN", .lang = "zh" },	/* China */
    { .icao = "ZW", .cc = "CN", .lang = "zh" },	/* China */
    { .icao = "ZY", .cc = "CN", .lang = "zh" },	/* China */
    { .icao = NULL, .cc = NULL, .lang = NULL}	/* Last entry */
};

/*
 * Converts an ICAO code to a country code. This performs a simple prefix
 * match using the icao2cc_table.
 */
const char *
icao2cc(const char *icao)
{
	/*
	 * Doing a linear search is not particularly elegant, but the size
	 * of the ICAO table is fixed and small, so it probably doesn't
	 * matter anyway.
	 */
	for (int i = 0; icao2cc_table[i].icao != NULL; i++) {
		if (strstr(icao, icao2cc_table[i].icao) == icao)
			return (icao2cc_table[i].cc);
	}

	return (NULL);
}

/*
 * Grabs a two-letter country code and tries to map it to language code of
 * the principal language spoken in that country. This shouldn't be relied
 * upon to be very accurate, since in reality the country-to-language
 * mapping is anything but clear cut.
 * Returns a two- or three-letter language code (if no two-letter one exists),
 * or NULL if no suitable mapping was found.
 */
const char *
cc2lang(const char *cc)
{
	for (int i = 0; icao2cc_table[i].cc != NULL; i++) {
		if (strcmp(cc, icao2cc_table[i].cc) == 0)
			return (icao2cc_table[i].lang);
	}
	return (NULL);
}
