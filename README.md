# mywiki
> tag 기반, 검색 프로그램

## 목적
"mywiki" 프로그램의 목적은 배웠던 내용을 쉽게 열람하는 데에 있다.  
리스킬링 과정 중 배웠던 내용이 너무나 방대하여, 시간이 지나면 배웠던 것들을 잊어버릴 수 있다.  
추후 위의 문제를 직면했을 때, 내가 배우고 작성했던 코드들을 쉽게 열람할 수 있다면 많은 도움이 될 것이라 생각한다.

## Compile 방법
1. 홈 디렉토리에 "jpg" 폴더 생성
1. "mywiki" 프로젝트 폴더를 "jpg"폴더로 복사 (path : /home/mobis/jpg/)  
  1-1. 만약 홈 디렉토리가 "/home/mobis/"이 아니라면, "mywiki/src/" 안에 있는 Makefile들에 작성된 DEFAULT_PATH를 바꿔주면 된다.  
3. "mywiki/src/"으로 이동하여 "make clean" 수행  
4. 같은 폴더에서 "make" 수행
5. "~/.bashrc" 파일 맨 아랫 부분에 "export LD_LIBRARY_PATH=/home/mobis/jpg/mywiki/lib:${LD_LIBRARY_PATH}" 추가
6. 터미널 창에서 "source ~/.bashrc" 실행

## 설명
"mywiki" 프로그램은 tag들을 기반으로 검색이 가능하도록 만들어졌다.  
DB에 미리 저장되어 있는 각 정보는 모두 1개 이상의 tag가 등록되어 있다.  
사용자는 1개 이상의 tag를 입력하여, 찾고자 하는 정보와 가장 밀접한 정보를 쉽게 검색할 수 있다.

## 기능
**Search:**
  - 입력한 tag들을 기반으로 DB에서 원하는 정보를 열람하기 위한 기능  
  - 열람한 소스 파일을 다운로드 받을 수 있다.

**Upload:**
  - 새로운 정보를 DB에 업로드하기 위한 기능  
 
**Revise:**
  - 입력한 tag들을 기반으로 DB에서 원하는 정보를 열람 후, 해당 정보를 수정하기 위한 기능  

**Delete:**
  - 입력한 tag들을 기반으로 DB에서 원하는 정보를 열람 후, 해당 정보를 삭제하기 위한 기능  


## Server 사용 방법
> server는 client의 요청에 따라, 검색/업로드/수정/삭제 등을 수행한 후 결과를 client에게 전달
1. "mywiki/bin/"으로 이동하여 "./server" 수행

## Client 사용 방법
> server에게 검색/업로드/수정/삭제 등을 요청

**Search :** server로부터 정보 열람 및 소스코드 다운로드를 요청 ("mywiki/data/client"에 저장)
1. "mywiki/bin/"으로 이동하여 "./client" 수행
2. 방향키로 조작하여, "Search" 실행
3. Tag들 등록 후, "Enter" 입력
4. 조회된 Title 중, 원하는 Title 입력

**Upload :** server에 새로운 정보를 전달하여 DB에 등록 ("mywiki/data/server/"에 저장)
1. "mywiki/bin/"으로 이동하여 "./client" 수행
2. 방향키로 조작하여, "Upload" 실행
3. 제목 입력
4. 내용 입력
5. Tag 등록

**Revise :** server에 이미 DB에 등록되어 있는 정보를 수정/전달하여 DB에 재등록 ("mywiki/data/server/"에 저장)
1. "mywiki/bin/"으로 이동하여 "./client" 수행
2. 방향키로 조작하여, "Revise" 실행
3. Tag들 등록 후, "Enter" 입력
4. 조회된 Title 중, 원하는 Title 입력
5. 제목 입력
6. 내용 입력
7. Tag 등록
 
**Delete :** server에 요청하여, DB에 등록되어 있는 정보를 삭제
1. "mywiki/bin/"으로 이동하여 "./client" 수행
2. 방향키로 조작하여, "Delete" 실행
3. Tag들 등록 후, "Enter" 입력
4. 조회된 Title 중, 원하는 Title 입력

**Exit :** "mywiki" 프로그램 종료
1. "mywiki/bin/"으로 이동하여 "./client" 수행
2. 방향키로 조작하여, "Exit" 입력
