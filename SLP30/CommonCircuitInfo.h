#pragma once

//#define		CIRCUIT_PARENT	19

//20220929 GBM - 아래 각 enum 정의가 문서 [연동 로직, 입출력 타입 정리_Ver 3(2022.05.19).xlsx] 내용과 불일치, enum 7 ~ 8, 12 ~ 13, 16 ~ 19, 26 ~ 29 추가, enum 42 ~ 45 삭제
enum {
	설비명_소화전 = 0,		// 0
	설비명_발신기,			// 1
	설비명_계단감지기,		// 2
	설비명_ELEV감지기,		// 3
	설비명_감지기,			// 4
	설비명_AN연기,			// 5
	설비명_AN정온,			// 6
	설비명_AN연기계단감지기,// 7
	설비명_AN연기ELEV감지기,// 8
	설비명_주소형연기,		// 9
	설비명_주소형정온,		// 10
	설비명_주소형차동,		// 11
	설비명_주소형연기계단감지기,		// 12
	설비명_주소형연기ELEV감지기,		// 13
	설비명_SVP감지기A,		// 14
	설비명_SVP감지기B,		// 15
	설비명_AN연기SVP감지기, // 16
	설비명_AN정온SVP감지기, // 17
	설비명_주소형차동SVP감지기A,	// 18
	설비명_주소형차동SVP감지기B,	// 19
	설비명_SVPP_S,			// 20
	설비명_SVPT_S,			// 21
	설비명_AVP_S,			// 22
	설비명_AVT_S,			// 23
	설비명_셔터연감지기,	// 24
	설비명_셔텨열감지기,	// 25
	설비명_AN연기셔터연감지기,		// 26
	설비명_AN정온셔터열감지기,		// 27
	설비명_주소형연기셔터연감지기,	// 28
	설비명_주소형정온셔터열감지기,	// 29
	설비명_셔터1차폐쇄,		// 30
	설비명_셔터2차폐쇄,		// 31
	설비명_전실급기댐퍼,	// 32
	설비명_전실급기수동,	// 33
	설비명_전실배기댐퍼,	// 34
	설비명_전실배기수동,	// 35
	설비명_자동폐쇄,		// 36
	설비명_창문폐쇄,		// 37
	설비명_배연창,			// 38
	설비명_방화문,			// 39
	설비명_P_L점등,			// 40
	설비명_유도등점등,		// 41
	//설비명_가스감지기A,	// 42
	//설비명_가스감지기B,	// 43
	//설비명_가스방출확인,	// 44
	//설비명_가스수동기동,	// 45
	설비명_저수위F_S,		// 42
	설비명_PUMPP_S,			// 43
	설비명_PUMPT_S,			// 44
	설비명_제연수동기동,	// 45
	설비명_SMD확인,			// 46
	설비명_MVD확인,			// 47
	설비명_급기팬,			// 48
	설비명_배기팬,			// 49
	설비명_저수위T_S,		// 50
	설비명_옥상문,			// 51
	설비명_시각경보,		// 52
	설비명_TS확인,			// 53
	설비명_하향식_피난사다리확인,	//54
};

static const TCHAR* g_lpszCircuitName[] = {
	_T("소화전"),		// 0
	_T("발신기"),		// 1
	_T("계단감지기"),	// 2
	_T("ELEV감지기"),	// 3
	_T("감지기"),		// 4
	_T("AN연기"),		// 5
	_T("AN정온"),		// 6
	_T("AN연기 계단감지기"),	// 7
	_T("AN연기 ELEV감지기"),	// 8
	_T("주소형연기"),	// 9
	_T("주소형정온"),	// 10
	_T("주소형차동"),	// 11
	_T("주소형연기 계단감지기"),		// 12
	_T("주소형연기 ELEV감지기"),		// 13
	_T("SVP감지기A"),		// 14
	_T("SVP감지기B"),		// 15
	_T("AN연기 SVP감지기"), // 16
	_T("AN정온 SVP감지기"), // 17
	_T("주소형차동 SVP감지기A"),	// 18
	_T("주소형차동 SVP감지기B"),	// 19
	_T("SVP P/S"), // 20
	_T("SVP T/S"), // 21
	_T("AV P/S"), // 22
	_T("AV T/S"), // 23
	_T("셔터연감지기"),		// 24
	_T("셔터열감지기"),		// 25
	_T("AN연기 셔터연감지기"),		// 26
	_T("AN정온 셔터열감지기"),		// 27
	_T("주소형연기 셔터연감지기"),		// 28
	_T("주소형정온 셔터열감지기"),	// 29
	_T("셔터1차폐쇄"),	// 30
	_T("셔터2차폐쇄"),	// 31
	_T("전실급기댐퍼"), // 32
	_T("전실급기수동"), // 33
	_T("전실배기댐퍼"),	// 34
	_T("전실배기수동"),		// 35
	_T("자동폐쇄"),		// 36
	_T("창문폐쇄"), // 37
	_T("배연창"),		// 38
	_T("방화문"),		// 39
	_T("P/L점등"),		// 40
	_T("유도등점등"),		// 41
	//_T("가스감지기A"),	// 42
	//_T("가스감지기B"),		// 43
	//_T("가스방출확인"),		// 44
	//_T("가스수동기동"), // 45
	_T("저수위F/S"), // 42
	_T("PUMP P/S"),	// 43
	_T("PUMP T/S"),		// 44
	_T("제연수동기동"),		// 45
	_T("SMD 확인"), // 46
	_T("MVD 확인"),		// 47
	_T("급기팬"),		// 48
	_T("배기팬"),		// 49
	_T("저수위T/S"),		// 50
	_T("옥상문"),	// 51
	_T("시각경보"),		// 52
	_T("TS 확인"),		// 53
	_T("하향식 피난사다리확인"),	// 54
	NULL };

static const TCHAR* g_lpszInputType[] = {
	_T("화재"),	// 0
	_T("화재"),	// 1
	_T("화재"),	// 2
	_T("화재"),	// 3
	_T("화재"),	// 4
	_T("AN연기"),	// 5
	_T("AN정온"),	// 6
	_T("AN연기"),	// 7
	_T("AN연기"),	// 8
	_T("주소형연기"),	// 9
	_T("주소형정온"),	// 10
	_T("주소형차동"),	// 11
	_T("주소형연기"),	// 12
	_T("주소형연기"),	// 13
	_T("SVP감지기A"),	// 14
	_T("SVP감지기B"),	// 15
	_T("AN연기"), // 16
	_T("AN정온"), // 17
	_T("주소형차동"),	// 18
	_T("주소형차동"),	// 19
	_T("화재"), // 20
	_T("감시"), // 21
	_T("화재"), // 22
	_T("감시"), // 23
	_T("화재"),	// 24
	_T("화재"),	// 25
	_T("AN연기"),	// 26
	_T("AN정온"),	// 27
	_T("주소형연기"),	// 28
	_T("주소형정온"),	// 29
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
	출력타입_지구경종 = 0, 
	출력타입_지구경종_, 
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
	출력타입_1차폐쇄,
	출력타입_2차폐쇄,
	출력타입_전실제연1,
	출력타입_33,
	출력타입_전실제연2,
	출력타입_35,
	출력타입_자동폐쇄,
	출력타입_창문폐쇄,
	출력타입_배연창,
	출력타입_방화문,
	출력타입_소화전기동,
	출력타입_유도등정지,
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
	_T("1차폐쇄"),	// 30
	_T("2차폐쇄"),	// 31
	_T("전실제연"), // 32
	_T(""), // 33
	_T("전실제연"),	// 34
	_T(""),	// 35
	_T("자동폐쇄"),	// 36
	_T("창문폐쇄"), // 37
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

static const TCHAR* g_lpszOutputType[] = {
	_T("지구음향"),	// 0
	_T("지구음향"),	// 1
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
	_T("방화셔터"),	// 30
	_T("방화셔터"),	// 31
	_T("전실제연"), // 32
	_T(""), // 33
	_T("전실제연"),	// 34
	_T(""),	// 35
	_T("출입문제어"),	// 36
	_T("창문제어"), // 37
	_T("창문제어"),	// 38
	_T("출입문제어"),	// 39
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
	_T("상가제연"), // 46
	_T("전실제연"),	// 47
	_T("제연팬"),	// 48
	_T("제연팬"),	// 49
	_T(""),	// 50
	_T("출입문제어"),	// 51
	_T("시각경보"),	// 52
	_T(""),	// 53
	_T(""),	// 54
	NULL
};

// CCircuitBasicInfo::CCircuitBasicInfo()
static const TCHAR* g_lpszCircuitParent[] = {
	_T("소화전"),			// 0
	_T("발신기"),			// 1
	_T("알람밸브"),			// 2
	_T("프리액션밸브"),		// 3
	_T("시각경보"),			// 4
	//_T("가스소화설비"),
	_T("전실제연급기"),		// 5
	_T("전실제연배기"),		// 6
	_T("상가제연"),			// 7
	_T("방화셔터"),			// 8
	_T("배연창"),			// 9
	_T("방화문"),			// 10
	_T("창폐장치"),			// 11
	_T("자폐장치"),			// 12
	_T("소화전기동확인"),	// 13
	_T("옥상개폐확인"),		// 14
	_T("저수위감시"),		// 15
	_T("제연급기팬"),		// 16
	_T("제연배기팬"),		// 17
	_T("감지기류"),			// 18
	_T("TS확인"),			// 19
	_T("하향식피난사다리확인"),			// 20
	NULL
};

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
	
	연동_셔터_연1_열2,		// 17
	연동_셔터_1차만,		// 18
	연동_셔터_2차만			// 19
};


