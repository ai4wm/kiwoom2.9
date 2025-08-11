# CLAUDE.md

이 파일은 Claude Code(claude.ai/code)가 이 리포지토리의 코드 작업 시 참고할 가이드를 제공합니다.

## 프로젝트 개요

키움증권 OpenAPI를 사용하는 MFC 기반 Windows 데스크톱 애플리케이션입니다. 주식 시세 조회, 주문, 실시간 데이터 수신 등의 기능을 제공합니다.

## 빌드 및 개발 명령어

### 빌드
- **Debug 빌드**: Visual Studio에서 `Debug|Win32` 구성으로 빌드
- **Release 빌드**: Visual Studio에서 `Release|Win32` 구성으로 빌드
- **명령줄**: `msbuild KhOpenApiTest.sln /p:Configuration=Debug` 또는 `Release`

### 실행
- 빌드된 실행파일: `Debug\KhOpenApiTest.exe` 또는 `Release\KhOpenApiTest.exe`
- **중요**: 관리자 권한 필요 (UAC RequireAdministrator 설정)

## 코드 아키텍처

### 핵심 구성 요소

1. **CKhOpenApiTestApp** (`KhOpenApiTest.cpp/.h`)
   - 메인 애플리케이션 클래스
   - 키움 OpenAPI 컨트롤 인스턴스 관리 (`m_khOpenApi`)
   - 데이터 포맷팅 및 에러 처리 유틸리티

2. **CKHOpenAPI** (`KHOpenAPICtrl.cpp/.h`)
   - 키움증권 ActiveX 컨트롤 래퍼 클래스
   - COM 인터페이스를 통한 증권사 API 호출
   - CLSID: `{A1574A0D-6BFA-4BD7-9020-DED88711818D}`

3. **CKhOpenApiTestDlg** (`KhOpenApiTestDlg.cpp/.h`)
   - 메인 다이얼로그 클래스
   - 화면 번호 관리 및 이벤트 핸들링
   - TR 데이터, 실시간 데이터, 체결 데이터 수신 처리

### 주요 다이얼로그 클래스
- **CurrentPriceDlg**: 현재가 조회
- **OrderDlg**: 주문 처리
- **KwanSimDlg**: 관심종목 관리
- **RateDlg**: 수익률 계산
- **RealAddDlg**: 실시간 데이터 등록

### GridCtrl 컴포넌트
- `GridCtrl\` 폴더의 그리드 컨트롤 라이브러리
- 데이터 표시용 테이블 UI 컴포넌트
- 셀 편집, 드래그앤드롭, 툴팁 기능 포함

## 개발 시 주의사항

### API 관련
- 키움증권 OpenAPI는 하루 사용량 제한이 있음
- 로그인 상태 확인 필요 (`GetConnectState()`)
- TR 코드별 호출 제한 (초당 5회)
- 화면 번호는 4자리 고유값 사용

### 에러 코드
프로젝트에서 정의된 주요 에러 코드들:
- `OP_ERR_SISE_OVERFLOW (-200)`: 시세조회 과부하
- `OP_ERR_NO_DATA (-203)`: 데이터 없음
- `OP_ERR_OVER_MAX_DATA (-204)`: 최대 조회 건수 초과
- `OP_ERR_ORD_WRONG_INPUT (-300)`: 주문 입력값 오류

### TR 코드
주요 사용 TR 코드:
- `OPT10001`: 주식기본정보요청
- `OPT10003`: 체결정보요청  
- `OPT10004`: 주식호가정보요청
- `OPT10080`: 주식분봉차트조회요청
- `OPT10081`: 주식일봉차트조회요청

### 데이터 타입 처리
`stGRID` 구조체의 데이터 타입:
- `DT_DATE (1)`: 날짜 형식
- `DT_TIME (2)`: 시간 형식  
- `DT_NUMBER (3)`: 숫자 형식
- `DT_SIGN (5)`: 부호 처리
- `DT_NUMBER_SIGN (6)`: 부호 포함 숫자

## 프로젝트 설정

- **플랫폼**: Win32 (x86)
- **문자셋**: MultiByte
- **MFC**: 동적 링크 사용
- **도구 집합**: v143 (Visual Studio 2022)
- **미리 컴파일된 헤더**: 사용 (`stdafx.h`)

## 리소스 파일

- `KhOpenApiTest.rc`: 대화상자 및 UI 리소스
- `res\KhOpenApiTest.ico`: 애플리케이션 아이콘
- `resource.h`: 리소스 ID 정의