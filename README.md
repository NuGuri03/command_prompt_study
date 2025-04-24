# C/C++ 명령 프롬프트 스터디

C 기초 학습을 바탕으로 커맨드 라인 쉘을 직접 구현하는 팀 프로젝트입니다.

---

## 🛠 Build 및 실행

```bash
make
./build/shell
```

---

## 팀원
|  이름  |                      GitHub ID                    |
|--------|---------------------------------------------------|
| 김민준 | [gangd0dan1014](https://github.com/gangd0dan1014) |
| 서유민 | [NuGuri03](https://github.com/NuGuri03) |
| 장기원 | [whitedev7773](https://github.com/whitedev7773) |

---

## 📦 기능 (1단계)
- 내부 명령어 처리:
  - `cd <dir>`: 디렉터리 이동
  - `pwd`: 현재 위치 출력
  - `exit`: 쉘 종료
- 외부 명령어 실행:
  - `ls`, `date` 등 리눅스 기본 명령어 실행
- 입력 파싱:
  - 문자열을 공백 기준으로 나눠 `argv` 배열 구성

---

## 📁 프로젝트 구조

```bash
myshell/
├── include/        # 헤더 파일 (.h)
│   ├── parser.h
│   └── executor.h
├── src/            # 소스 코드 (.c)
│   ├── main.c
│   ├── parser.c
│   └── executor.c
├── build/          # 빌드된 결과물 (.o, shell 실행 파일)
├── test/           # 단위 테스트용 코드
├── Makefile        # 빌드 자동화
└── README.md
```

## 🔜 다음 단계 예정 기능

- 파이프 (`|`)
- 리디렉션 (`>`, `<`)
- 백그라운드 실행 (`&`)
- C++ 도입 및 구조 리팩토링
- 명령어 히스토리 저장
- `.myshellrc` 설정파일 지원

---

## 🤝 GitHub 협업 방식

- 기능 단위 브랜치 (`feature/parser`, `feature/executor` 등) 생성
- PR → 리뷰 → 병합
- 커밋 메시지는 `feat:`, `fix:`, `docs:` 등 [Conventional Commits] 스타일 사용
- `build/`, `*.o` 파일 등은 `.gitignore`로 관리

---