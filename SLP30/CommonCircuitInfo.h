#pragma once

//#define		CIRCUIT_PARENT	19

//20220929 GBM - 아래 각 enum 정의가 문서 [연동 로직, 입출력 타입 정리_Ver 3(2022.05.19).xlsx] 내용과 불일치, enum 7 ~ 8, 12 ~ 13, 16 ~ 19, 26 ~ 29 추가, enum 42 ~ 45 삭제
enum {
	설비명_소화전 = 0,		// 0
	설비명_발신기,			// 1
	설비명_감지기_계단,		// 2
	설비명_감지기_ELEV,		// 3
	설비명_감지기,			// 4
	설비명_아날로그_광전,	// 5
	설비명_아날로그_정온,	// 6
	설비명_아날로그_광전_계단,// 7
	설비명_아날로그_광전_ELEV,// 8
	설비명_주소형_광전,		// 9
	설비명_주소형_정온,		// 10
	설비명_주소형_차동,		// 11
	설비명_주소형_광전_계단,		// 12
	설비명_주소형_광전_ELEV,		// 13
	설비명_감지기_A_SVP,		// 14
	설비명_감지기_B_SVP,		// 15
	설비명_아날로그_광전_SVP, // 16
	설비명_아날로그_정온_SVP, // 17
	설비명_주소형_차동_감지기_A_SVP,	// 18
	설비명_주소형_차동_감지기_B_SVP,	// 19
	설비명_P_S_확인_SVP,			// 20
	설비명_P_T_확인_SVP,			// 21
	설비명_알람밸브_P_S,			// 22
	설비명_알람밸브_T_S,			// 23
	설비명_광전_셔터,	// 24
	설비명_광온_셔터,	// 25
	설비명_아날로그_광전_셔터,		// 26
	설비명_아날로그_정온_셔터,		// 27
	설비명_주소형_광전_셔터,	// 28
	설비명_주소형_정온_셔터,	// 29
	설비명_셔터_1차_폐쇄,		// 30
	설비명_셔터_2차_폐쇄,		// 31
	설비명_전실_급기댐퍼,	// 32
	설비명_전실_급기수동,	// 33
	설비명_전실_배기댐퍼,	// 34
	설비명_전실_배기수동,	// 35
	설비명_자동_폐쇄,		// 36
	설비명_창문_폐쇄,		// 37
	설비명_배연창,			// 38
	설비명_방화문,			// 39
	설비명_P_L_점등,			// 40
	설비명_유도등_점등,		// 41
	//설비명_가스감지기A,	// 42
	//설비명_가스감지기B,	// 43
	//설비명_가스방출확인,	// 44
	//설비명_가스수동기동,	// 45
	설비명_저수위_F_S,		// 42
	설비명_P_S_확인_PUMP,			// 43
	설비명_T_S_확인_PUMP,			// 44
	설비명_제연_수동기동,	// 45
	설비명_상가댐퍼_확인,			// 46
	설비명_복합댐퍼_확인,			// 47
	설비명_급기팬,			// 48
	설비명_배기팬,			// 49
	설비명_저수위_T_S,		// 50
	설비명_옥상문,			// 51
	설비명_시각경보,		// 52
	설비명_T_S_확인,			// 53
	설비명_피난사다리확인,	//54
};

#ifndef ENGLISH_MODE
static const TCHAR* g_lpszCircuitName[] = {
	_T("소화전"),		// 0
	_T("발신기"),		// 1
	_T("감지기 (계단)"),	// 2
	_T("감지기 (ELEV)"),	// 3
	_T("감지기"),		// 4
	_T("아날로그 광전"),		// 5
	_T("아날로그 정온"),		// 6
	_T("아날로그 광전 (계단)"),	// 7
	_T("아날로그 광전 (ELEV)"),	// 8
	_T("주소형 광전"),	// 9
	_T("주소형 정온"),	// 10
	_T("주소형 차동"),	// 11
	_T("주소형 광전 (계단)"),		// 12
	_T("주소형 광전 (ELEV)"),		// 13
	_T("감지기 A (SVP)"),		// 14
	_T("감지기 B (SVP)"),		// 15
	_T("아날로그 광전 (SVP)"), // 16
	_T("아날로그 정온 (SVP)"), // 17
	_T("주소형 차동 감지기 A (SVP)"),	// 18
	_T("주소형 차동 감지기 B (SVP)"),	// 19
	_T("P/S 확인 (SVP)"), // 20
	_T("T/S 확인 (SVP)"), // 21
	_T("알람밸브 P/S"), // 22
	_T("알람밸브 T/S"), // 23
	_T("광전 (셔터)"),		// 24
	_T("정온 (셔터)"),		// 25
	_T("아날로그 광전 (셔터)"),		// 26
	_T("아날로그 정온 (셔터)"),		// 27
	_T("주소형 광전 (셔터)"),		// 28
	_T("주소형 정온 (셔터)"),	// 29
	_T("셔터 1차 폐쇄"),	// 30
	_T("셔터 2차 폐쇄"),	// 31
	_T("전실 급기댐퍼"), // 32
	_T("전실 급기수동"), // 33
	_T("전실 배기댐퍼"),	// 34
	_T("전실 배기수동"),		// 35
	_T("자동 폐쇄"),		// 36
	_T("창문 폐쇄"), // 37
	_T("배연창"),		// 38
	_T("방화문"),		// 39
	_T("P/L 점등"),		// 40
	_T("유도등 점등"),		// 41
	//_T("가스감지기A"),	// 42
	//_T("가스감지기B"),		// 43
	//_T("가스방출확인"),		// 44
	//_T("가스수동기동"), // 45
	_T("저수위 F/S"), // 42
	_T("P/S 확인 (PUMP)"),	// 43
	_T("T/S 확인 (PUMP)"),		// 44
	_T("제연 수동기동"),		// 45
	_T("상가댐퍼 확인"), // 46
	_T("복합댐퍼 확인"),		// 47
	_T("급기팬"),		// 48
	_T("배기팬"),		// 49
	_T("저수위 T/S"),		// 50
	_T("옥상문"),	// 51
	_T("시각경보"),		// 52
	_T("T/S 확인"),		// 53
	_T("피난사다리 확인"),	// 54
	NULL };
#else
static const TCHAR* g_lpszCircuitName[] = {
	_T("HYDR"),		// 0
	_T("M.STAT"),		// 1
	_T("DET (STA)"),	// 2
	_T("DET (ELV)"),	// 3
	_T("DET"),		// 4
	_T("AN SMK"),		// 5
	_T("AN HFD"),		// 6
	_T("AN SMK (STA)"),	// 7
	_T("AN SMK (ELV)"),	// 8
	_T("AD SMK"),	// 9
	_T("AD HFD"),	// 10
	_T("AD ROR"),	// 11
	_T("AD SMK (STA)"),		// 12
	_T("AD SMK (ELV)"),		// 13
	_T("DA (SVP)"),		// 14
	_T("DB (SVP)"),		// 15
	_T("AN SMK (SVP)"), // 16
	_T("AN HFD (SVP)"), // 17
	_T("AD RORA (SVP)"),	// 18
	_T("AD RORB (SVP)"),	// 19
	_T("SVP P/S"), // 20
	_T("SVP T/S"), // 21
	_T("AV P/S"), // 22
	_T("AV T/S"), // 23
	_T("SMK (SHT)"),		// 24
	_T("HFD (SHT)"),		// 25
	_T("AN SMK (SHT)"),		// 26
	_T("AN HFD (SHT)"),		// 27
	_T("AD SMK (SHT)"),		// 28
	_T("AD HFD (SHT)"),	// 29
	_T("SHT (1ST)"),	// 30
	_T("SHT (2ND)"),	// 31
	_T("SD (VEST)"), // 32
	_T("SDM (VEST)"), // 33
	_T("ED (VEST)"),	// 34
	_T("EDM (VEST)"),		// 35
	_T("A.DOCL"),		// 36
	_T("W.CLOS"),		// 37
	_T("WIND"),		// 38
	_T("DOOR"),		// 39
	_T("P/L ON"),		// 40
	_T("E/S ON"),		// 41	//EMERGENCY EXIT SIGN ON -> 사용하지 않는 걸로 보임
	//_T("가스감지기A"),	// 42
	//_T("가스감지기B"),		// 43
	//_T("가스방출확인"),		// 44
	//_T("가스수동기동"), // 45
	_T("LW F/S"), // 42
	_T("PUMP P/S"),	// 43
	_T("PUMP T/S"),		// 44
	_T("SMDM ACT"),		// 45
	_T("HSMD ACT"), // 46
	_T("MVD ACT"),		// 47
	_T("S.FAN"),		// 48
	_T("E.FAN"),		// 49
	_T("LW T/S"),		// 50
	_T("R.DOOR"),	// 51
	_T("STROBE"),		// 52
	_T("T/S ACT"),		// 53
	_T("E/L ACT"),	// 54
	NULL };
#endif

#ifndef ENGLISH_MODE
static const TCHAR* g_lpszInputType[] = {
	_T("화재"),	// 0
	_T("화재"),	// 1
	_T("화재"),	// 2
	_T("화재"),	// 3
	_T("화재"),	// 4
	_T("아날로그 광전"),	// 5
	_T("아날로그 정온"),	// 6
	_T("아날로그 광전"),	// 7
	_T("아날로그 광전"),	// 8
	_T("주소형 광전"),	// 9
	_T("주소형 정온"),	// 10
	_T("주소형 차동"),	// 11
	_T("주소형 광전"),	// 12
	_T("주소형 광전"),	// 13
	_T("감지기 A (SVP)"),	// 14
	_T("감지기 B (SVP)"),	// 15
	_T("아날로그 광전"), // 16
	_T("아날로그 정온"), // 17
	_T("주소형 차동"),	// 18
	_T("주소형 차동"),	// 19
	_T("화재"), // 20
	_T("감시"), // 21
	_T("화재"), // 22
	_T("감시"), // 23
	_T("화재"),	// 24
	_T("화재"),	// 25
	_T("아날로그 광전"),	// 26
	_T("아날로그 정온"),	// 27
	_T("주소형 광전"),	// 28
	_T("주소형 정온"),	// 29
	_T("감시"),	// 30
	_T("감시"),	// 31
	_T("감시"), // 32
	_T("화재"), // 33
	_T("감시"),	// 34
	_T("화재"),	// 35
	_T("감시"),	// 36
	_T("감시"), // 37
	_T("감시"),	// 38
	_T("감시"),	// 39
	_T("감시"),	// 40
	_T("감시"),	// 41
	//_T("화재"),	// 42
	//_T("화재"),	// 43
	//_T("화재"),	// 44
	//_T("화재"), // 45
	_T("감시"), // 42
	_T("감시"),	// 43
	_T("감시"),	// 44
	_T("화재"),	// 45
	_T("감시"), // 46
	_T("감시"),	// 47
	_T("감시"),	// 48
	_T("감시"),	// 49
	_T("감시"),	// 50
	_T("감시"),	// 51
	_T("감시"),	// 52
	_T("감시"),	// 53
	_T("감시"),	// 54
	NULL };
#else
static const TCHAR* g_lpszInputType[] = {
	_T("ALARM"),	// 0
	_T("ALARM"),	// 1
	_T("ALARM"),	// 2
	_T("ALARM"),	// 3
	_T("ALARM"),	// 4
	_T("AN SMK"),	// 5
	_T("AN HFD"),	// 6
	_T("AN SMK"),	// 7
	_T("AN SMK"),	// 8
	_T("AD SMK"),	// 9
	_T("AD HFD"),	// 10
	_T("AD ROR"),	// 11
	_T("AD SMK"),	// 12
	_T("AD SMK"),	// 13
	_T("DA (SVP)"),	// 14
	_T("DB (SVP)"),	// 15
	_T("AN SMK"), // 16
	_T("AN HFD"), // 17
	_T("AD ROR"),	// 18
	_T("AD ROR"),	// 19
	_T("ALARM"), // 20
	_T("SUPV"), // 21
	_T("ALARM"), // 22
	_T("SUPV"), // 23
	_T("ALARM"),	// 24
	_T("ALARM"),	// 25
	_T("AN SMK"),	// 26
	_T("AN HFD"),	// 27
	_T("AD SMK"),	// 28
	_T("AD HFD"),	// 29
	_T("SUPV"),	// 30
	_T("SUPV"),	// 31
	_T("SUPV"), // 32
	_T("ALARM"), // 33
	_T("SUPV"),	// 34
	_T("ALARM"),	// 35
	_T("SUPV"),	// 36
	_T("SUPV"), // 37
	_T("SUPV"),	// 38
	_T("SUPV"),	// 39
	_T("SUPV"),	// 40
	_T("SUPV"),	// 41
	//_T("화재"),	// 42
	//_T("화재"),	// 43
	//_T("화재"),	// 44
	//_T("화재"), // 45
	_T("SUPV"), // 42
	_T("SUPV"),	// 43
	_T("SUPV"),	// 44
	_T("ALARM"),	// 45
	_T("SUPV"), // 46
	_T("SUPV"),	// 47
	_T("SUPV"),	// 48
	_T("SUPV"),	// 49
	_T("SUPV"),	// 50
	_T("SUPV"),	// 51
	_T("SUPV"),	// 52
	_T("SUPV"),	// 53
	_T("SUPV"),	// 54
	NULL };
#endif

static const TCHAR* g_lpszInputNo[] = {
	_T("1"),	// 0
	_T("1"),	// 1
	_T("1"),	// 2
	_T("1"),	// 3
	_T("1"),	// 4
	_T("7"),	// 5
	_T("6"),	// 6
	_T("7"),	// 7
	_T("7"),	// 8
	_T("9"),	// 9
	_T("8"),	// 10
	_T("8"),	// 11
	_T("9"),	// 12
	_T("9"),	// 13
	_T("2"),	// 14
	_T("3"),	// 15
	_T("7"),	// 16
	_T("6"),	// 17
	_T("9"),	// 18
	_T("8"),	// 19
	_T("5"),	// 20
	_T("10"),	// 21
	_T("5"),	// 22
	_T("10"),	// 23
	_T("1"),	// 24
	_T("1"),	// 25
	_T("7"),	// 26
	_T("6"),	// 27
	_T("9"),	// 28
	_T("8"),	// 29
	_T("10"),	// 30
	_T("10"),	// 31
	_T("10"),	// 32
	_T("5"),	// 33
	_T("10"),	// 34
	_T("5"),	// 35
	_T("10"),	// 36
	_T("10"),	// 37
	_T("10"),	// 38
	_T("10"),	// 39
	_T("12"),	// 40
	_T("12"),	// 41
	//_T("1"),	// 42
	//_T("1"),	// 43
	//_T("5"),	// 44
	//_T("5"),	// 45
	_T("10"),	// 42
	_T("10"),	// 43
	_T("10"),	// 44
	_T("5"),	// 45
	_T("10"),	// 46
	_T("10"),	// 47
	_T("10"),	// 48
	_T("10"),	// 49
	_T("10"),	// 50
	_T("10"),	// 51
	_T("12"),	// 52
	_T("10"),	// 53
	_T("10"),	// 54
	NULL };

static const TCHAR* g_lpszOutputNo[] = {
	_T("2"),	// 0
	_T("2"),	// 1
	_T("0"),	// 2
	_T("0"),	// 3
	_T("0"),	// 4
	_T("0"),	// 5
	_T("0"),	// 6
	_T("0"),	// 7
	_T("0"),	// 8
	_T("0"),	// 9
	_T("0"),	// 10
	_T("0"),	// 11
	_T("0"),	// 12
	_T("0"),	// 13
	_T("3"),	// 14
	_T("4"),	// 15
	_T("0"),	// 16
	_T("0"),	// 17
	_T("0"),	// 18
	_T("0"),	// 19
	_T("0"),	// 20
	_T("0"),	// 21
	_T("0"),	// 22
	_T("0"),	// 23
	_T("0"),	// 24
	_T("0"),	// 25
	_T("0"),	// 26
	_T("0"),	// 27
	_T("0"),	// 28
	_T("0"),	// 29
	_T("7"),	// 30
	_T("7"),	// 31
	_T("8"),	// 32
	_T("0"),	// 33
	_T("8"),	// 34
	_T("0"),	// 35
	_T("6"),	// 36
	_T("5"),	// 37
	_T("5"),	// 38
	_T("6"),	// 39
	_T("0"),	// 40
	_T("0"),	// 41
	//_T("0"),	// 42
	//_T("0"),	// 43
	//_T("0"),	// 44
	//_T("0"),	// 45
	_T("0"),	// 42
	_T("0"),	// 43
	_T("0"),	// 44
	_T("0"),	// 45
	_T("12"),	// 46
	_T("8"),	// 47
	_T("9"),	// 48
	_T("9"),	// 49
	_T("0"),	// 50
	_T("6"),	// 51
	_T("11"),	// 52
	_T("0"),	// 53
	_T("0"),	// 54
	NULL };

enum {
	출력타입_지구_경종 = 0, 
	출력타입_지구_경종_, 
	출력타입_2, 
	출력타입_3, 
	출력타입_4, 
	출력타입_5, 
	출력타입_6, 
	출력타입_7, 
	출력타입_8,
	출력타입_9,
	출력타입_10,
	출력타입_11,
	출력타입_12,
	출력타입_13,
	출력타입_14,
	출력타입_15,
	출력타입_16,
	출력타입_17,
	출력타입_18,
	출력타입_19,
	출력타입_싸이렌,
	출력타입_밸브,
	출력타입_22,
	출력타입_23,
	출력타입_24,
	출력타입_25,
	출력타입_26,
	출력타입_27,
	출력타입_28,
	출력타입_29,
	출력타입_셔터_1차_폐쇄,
	출력타입_셔터_2차_폐쇄,
	출력타입_전실제연_1,
	출력타입_33,
	출력타입_전실제연_2,
	출력타입_35,
	출력타입_자동_폐쇄,
	출력타입_창문_폐쇄,
	출력타입_배연창,
	출력타입_방화문,
	출력타입_소화전_기동,
	출력타입_유도등_정지,
	/*출력타입_42,
	출력타입_43,
	출력타입_44,
	출력타입_45,*/
	출력타입_42,
	출력타입_43,
	출력타입_44,
	출력타입_45,
	출력타입_거실댐퍼,		// 38,
	출력타입_복합댐퍼,		// 39,
	출력타입_급기팬,		// 40,
	출력타입_배기팬,		// 41,
	출력타입_50,
	출력타입_옥상문,		// 43,
	출력타입_시각경보,		// 44,
	출력타입_53,
	출력타입_54,
};

#ifndef ENGLISH_MODE
static const TCHAR* g_lpszOutputCircuit[] = {
	_T("지구 경종"),	// 0
	_T("지구 경종"),	// 1
	_T(""),	// 2
	_T(""),	// 3
	_T(""),	// 4
	_T(""),	// 5
	_T(""),	// 6
	_T(""),	// 7
	_T(""),	// 8
	_T(""),	// 9
	_T(""),	// 10
	_T(""),	// 11
	_T(""),	// 12
	_T(""),	// 13
	_T(""),	// 14
	_T(""),	// 15
	_T(""), // 16
	_T(""), // 17
	_T(""),	// 18
	_T(""),	// 19
	_T("싸이렌"), // 20
	_T("밸브"), // 21
	_T(""), // 22
	_T(""), // 23
	_T(""),	// 24
	_T(""),	// 25
	_T(""),	// 26
	_T(""),	// 27
	_T(""),	// 28
	_T(""),	// 29
	_T("셔터 1차 폐쇄"),	// 30
	_T("셔터 2차 폐쇄"),	// 31
	_T("전실 제연"), // 32
	_T(""), // 33
	_T("전실 제연"),	// 34
	_T(""),	// 35
	_T("자동 폐쇄"),	// 36
	_T("창문 폐쇄"), // 37
	_T("배연창"),	// 38
	_T("방화문"),	// 39
	_T("소화전 기동"),	// 40
	_T("유도등 정지"),	// 41
	//_T(""),	// 42
	//_T(""),	// 43
	//_T(""),	// 44
	//_T(""), // 45
	_T(""), // 42
	_T(""),	// 43
	_T(""),	// 44
	_T(""),	// 45
	_T("거실댐퍼"), // 46
	_T("복합댐퍼"),	// 47
	_T("급기팬"),	// 48
	_T("배기팬"),	// 49
	_T(""),	// 50
	_T("옥상문"),	// 51
	_T("시각경보"),	// 52
	_T(""),	// 53
	_T(""),	// 54
	NULL };
#else
static const TCHAR* g_lpszOutputCircuit[] = {
	_T("L.BELL"),	// 0
	_T("L.BELL"),	// 1
	_T(""),	// 2
	_T(""),	// 3
	_T(""),	// 4
	_T(""),	// 5
	_T(""),	// 6
	_T(""),	// 7
	_T(""),	// 8
	_T(""),	// 9
	_T(""),	// 10
	_T(""),	// 11
	_T(""),	// 12
	_T(""),	// 13
	_T(""),	// 14
	_T(""),	// 15
	_T(""), // 16
	_T(""), // 17
	_T(""),	// 18
	_T(""),	// 19
	_T("SIREN"), // 20
	_T("VALVE"), // 21
	_T(""), // 22
	_T(""), // 23
	_T(""),	// 24
	_T(""),	// 25
	_T(""),	// 26
	_T(""),	// 27
	_T(""),	// 28
	_T(""),	// 29
	_T("SHT (1ST)"),	// 30
	_T("SHT (2ND"),	// 31
	_T("SMC (VEST)"), // 32
	_T(""), // 33
	_T("SMC (VEST"),	// 34
	_T(""),	// 35
	_T("A.DOCL"),	// 36
	_T("W.CLOS"), // 37
	_T("WIND"),	// 38
	_T("DOOR"),	// 39
	_T("HYDR ON"),	// 40
	_T("E/S OFF"),	// 41
	//_T(""),	// 42
	//_T(""),	// 43
	//_T(""),	// 44
	//_T(""), // 45
	_T(""), // 42
	_T(""),	// 43
	_T(""),	// 44
	_T(""),	// 45
	_T("LRMD"), // 46
	_T("MVD"),	// 47
	_T("S.FAN"),	// 48
	_T("E.FAN"),	// 49
	_T(""),	// 50
	_T("R.DOOR"),	// 51
	_T("STROBE"),	// 52
	_T(""),	// 53
	_T(""),	// 54
	NULL };
#endif

#ifndef ENGLISH_MODE
static const TCHAR* g_lpszOutputType[] = {
	_T("지구 경종"),	// 0
	_T("지구 경종"),	// 1
	_T(""),	// 2
	_T(""),	// 3
	_T(""),	// 4
	_T(""),	// 5
	_T(""),	// 6
	_T(""),	// 7
	_T(""),	// 8
	_T(""),	// 9
	_T(""),	// 10
	_T(""),	// 11
	_T(""),	// 12
	_T(""),	// 13
	_T(""),	// 14
	_T(""),	// 15
	_T(""), // 16
	_T(""), // 17
	_T(""),	// 18
	_T(""),	// 19
	_T("싸이렌"), // 20
	_T("프리액션"), // 21
	_T(""), // 22
	_T(""), // 23
	_T(""),	// 24
	_T(""),	// 25
	_T(""),	// 26
	_T(""),	// 27
	_T(""),	// 28
	_T(""),	// 29
	_T("방화 셔터"),	// 30
	_T("방화 셔터"),	// 31
	_T("전실 제연"), // 32
	_T(""), // 33
	_T("전실 제연"),	// 34
	_T(""),	// 35
	_T("출입문 제어"),	// 36
	_T("창문 제어"), // 37
	_T("창문 제어"),	// 38
	_T("출입문 제어"),	// 39
	_T("기타"),	// 40
	_T("기타"),	// 41
	//_T(""),	// 42
	//_T(""),	// 43
	//_T(""),	// 44
	//_T(""), // 45
	_T(""), // 42
	_T(""),	// 43
	_T(""),	// 44
	_T(""),	// 45
	_T("상가 제연"), // 46
	_T("전실 제연"),	// 47
	_T("제연팬"),	// 48
	_T("제연팬"),	// 49
	_T(""),	// 50
	_T("출입문 제어"),	// 51
	_T("시각경보"),	// 52
	_T(""),	// 53
	_T(""),	// 54
	NULL
};
#else
static const TCHAR* g_lpszOutputType[] = {
	_T("L.BELL"),	// 0
	_T("L.BELL"),	// 1
	_T(""),	// 2
	_T(""),	// 3
	_T(""),	// 4
	_T(""),	// 5
	_T(""),	// 6
	_T(""),	// 7
	_T(""),	// 8
	_T(""),	// 9
	_T(""),	// 10
	_T(""),	// 11
	_T(""),	// 12
	_T(""),	// 13
	_T(""),	// 14
	_T(""),	// 15
	_T(""), // 16
	_T(""), // 17
	_T(""),	// 18
	_T(""),	// 19
	_T("SIREN"), // 20
	_T("P-ACT"), // 21
	_T(""), // 22
	_T(""), // 23
	_T(""),	// 24
	_T(""),	// 25
	_T(""),	// 26
	_T(""),	// 27
	_T(""),	// 28
	_T(""),	// 29
	_T("SHT"),	// 30
	_T("SHT"),	// 31
	_T("SMC (VEST)"), // 32
	_T(""), // 33
	_T("SMC (VEST)"),	// 34
	_T(""),	// 35
	_T("D.CNTR"),	// 36
	_T("W.CNTR"), // 37
	_T("W.CNTR"),	// 38
	_T("D.CNTR"),	// 39
	_T("ETC"),	// 40
	_T("ETC"),	// 41
	//_T(""),	// 42
	//_T(""),	// 43
	//_T(""),	// 44
	//_T(""), // 45
	_T(""), // 42
	_T(""),	// 43
	_T(""),	// 44
	_T(""),	// 45
	_T("HSC (VEST)"), // 46
	_T("SMC (VEST)"),	// 47
	_T("C.FAN"),	// 48
	_T("C.FAN"),	// 49
	_T(""),	// 50
	_T("D.CNTR"),	// 51
	_T("STROBE"),	// 52
	_T(""),	// 53
	_T(""),	// 54
	NULL
	};
#endif

#ifndef ENGLISH_MODE
// CCircuitBasicInfo::CCircuitBasicInfo()
static const TCHAR* g_lpszCircuitParent[] = {
	_T("소화전"),			// 0
	_T("발신기"),			// 1
	_T("알람밸브"),			// 2
	_T("프리액션밸브"),		// 3
	_T("시각경보"),			// 4
	//_T("가스소화설비"),
	_T("전실 제연 급기"),		// 5
	_T("전실 제연 배기"),		// 6
	_T("상가 제연"),			// 7
	_T("방화 셔터"),			// 8
	_T("배연창"),			// 9
	_T("방화문"),			// 10
	_T("창폐 장치"),			// 11
	_T("자폐 장치"),			// 12
	_T("소화전 기동 확인"),	// 13
	_T("옥상 개폐 확인"),		// 14
	_T("저수위 감시"),		// 15
	_T("제연 급기팬"),		// 16
	_T("제연 배기팬"),		// 17
	_T("감지기류"),			// 18
	_T("T/S 확인"),			// 19
	_T("피난사다리 확인"),			// 20
	NULL
};
#else
// CCircuitBasicInfo::CCircuitBasicInfo()
static const TCHAR* g_lpszCircuitParent[] = {
	_T("HYDR"),			// 0
	_T("M.STAT"),			// 1
	_T("A.VALV"),			// 2
	_T("PA VALV"),		// 3
	_T("STROBE"),			// 4
	//_T("가스소화설비"),
	_T("SCC (VEST)"),		// 5
	_T("ESC (VEST)"),		// 6
	_T("HSC (VEST)"),			// 7
	_T("SHT"),			// 8
	_T("WIND"),			// 9
	_T("DOOR"),			// 10
	_T("W.CLOS"),			// 11
	_T("A.DOCL"),			// 12
	_T("HYDR ON"),	// 13
	_T("RDOOR ACT"),		// 14
	_T("LW ACT"),		// 15
	_T("S.FAN"),		// 16
	_T("E.FAN"),		// 17
	_T("DET"),			// 18
	_T("T/S ACT"),			// 19
	_T("E/L ACT"),			// 20
	NULL
};
#endif

enum {
	연동_경종_전층 = 0, 
	연동_경종_계단별직상층,	// 1
	연동_경종_동별직상층,	// 2

	연동_밸브_교차,			// 3
	연동_밸브_아날로그,		// 4
	
	연동_싸이렌_작동연동,	// 5
	연동_싸이렌_동작확인, 
	
	연동_문_전층,			// 7
	연동_문_계단별전층, 
	
	연동_배연창_건물해당층,	// 9
	연동_배연창_계단별해당층, 
	
	연동_급기_건물전층,		// 11
	연동_급기_계단별전층, 
	
	연동_배기_건물해당층,	// 13
	연동_배기_계단별해당층, 
	
	연동_급기팬_계단별,		// 15
	연동_배기팬_계단별, 
	
	연동_셔터_광전1_정온2,		// 17
	연동_셔터_1차만,		// 18
	연동_셔터_2차만			// 19
};


