#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <thread> // sleep
#include <chrono> // sleep
class Monster;

// Player : 플레이어 부모 클래스 (체력 마나 공포 돈 가방)
class Player {
protected:
	int fear;
	int gold;
	int mana;
	int maxMana;    
	int hp;
	int maxHp;      
	int luck;
	int skillatk;
	int atk;

	std::string kil;
	std::vector<std::string> bag; // 가방 (vector)
public:
	Player(int h, int m, int a, int s) {
		hp = h;
		maxHp = h;
		mana = m;
		maxMana = m;
		atk = a;
		skillatk = s;
		fear = 0;
		gold = 60;
		luck = rand() % 11;	
	}
	virtual ~Player() {}

	int getatk() { return atk; }
	int getLuck() { return luck; }
	int getFear() { return fear; }

	// 증강할때 씀
	void addMaxHp(int a) { maxHp += a; hp += a; }

	void addMaxMana(int a) { maxMana += a; mana += a; }

	void addAtk(int a) { atk += a; }

	void addSkillAtk(int a) { skillatk += a; }

	void addLuck(int a) { luck += a; }

	virtual bool skill1(Monster& m) = 0; 

	virtual bool skill2(Monster& m) = 0;

	void heal(int amount) {
		hp += amount;
		if (hp > maxHp) hp = maxHp; // 최대체력 넘으면 안됨
		std::cout << "체력 " << amount << " 회복!\n";
	}
	void addMana(int amount) {
		mana += amount;
		if (mana > maxMana) mana = maxMana; // 최대마나 넘으면 안됨
	}
	void addFear(int amount) {
		fear += amount;
		if (fear < 0) fear = 0;
		if (fear > 100) fear = 100;
	}
	// 공포 숫자는 안보여주고 글로만
	std::string fearText() {
		if (fear < 25) return "침착함";
		if (fear < 50) return "불안함";
		if (fear < 80) return "두려움";
		return "공포에 질림";
	}

	void printStatus() {
		std::cout << "==========================\n";
		std::cout << "HP: " << hp << "/" << maxHp << "  마나: " << mana << "/" << maxMana << "\n";
		std::cout << "정신 상태: " << fearText() << "\n";
		std::cout << "공격력: " << atk << "  스킬 공격력: " << skillatk << "\n";
		std::cout << "돈: " << gold << "G  가방: " << bag.size() << "개\n";
		std::cout << "==========================\n";
	}

	void takedamge(int dmg, std::string monmonster) {
		if (rand() % 100 < luck) { // 회피 (운)
			std::cout << "운 좋게 피했다!\n";
			return;
		}
		hp -= dmg;
		std::cout << dmg << " 피해를 입었다!\n";
		addFear(2); // 맞으면 공포 올라감
		if (hp < 0) hp = 0;
		if (hp == 0) {
			kil = monmonster;
			std::cout << kil << "에게 죽임을 당했습니다\n";
		}


	}
	//add item
	void additem(std::string item) {
		bag.push_back(item);
		std::cout << item << " 획득\n";
	}

	bool useItem() {
		if (bag.size() == 0) {
			std::cout << "가방이 비어 있습니다.\n";
			return false;
		}

		// 가방 보여주기
		for (int i = 0; i < bag.size(); i++) {
			std::cout << i + 1 << ". " << bag[i] << "\n";
		}

		int c;
		std::cout << "사용할 아이템 번호: ";
		std::cin >> c;
		if (c < 1 || c > bag.size()) { 
			std::cout << "잘못된 번호입니다.\n";
			return false;
		}

		std::string item = bag[c - 1];
		if (item == "체력 포션") {
			heal(40);
		}
		else if (item == "마나 포션") {
			addMana(30);
			std::cout << "마나 30 회복!\n";
		}
		else if (item == "진정제") {
			addFear(-25);
			std::cout << "마음이 차분해진다...\n";
		}

		bag.erase(bag.begin() + (c - 1));
		return true;
	}
	int getGold() { return gold; }
	void spendGold(int price) { gold -= price; }
	void addGold(int g) {
		gold += g;
		std::cout << g << "G 획득!\n";
	}
	bool life() { return hp > 0; }
};



// Monster : 몬스터 부모 클래스
class Monster {
protected:
	std::string name;
	int hp;
	int atk;
	int gold;

public:
	Monster(std::string n, int h, int a, int g) {
		name = n;
		hp = h;
		atk = a;
		gold = g;
	}
	virtual ~Monster() {}
	void printStatus() {
		std::cout << "[" << name << "] HP: " << hp << "\n";
	}
	void takeDamage(int dmg) {
		hp -= dmg;
		if (hp < 0) hp = 0;
	}

	
	void operator-=(int dmg) {
		takeDamage(dmg);
	}
	virtual void skill(Player& p) {} 

	int getGold() { return gold; }
	bool isAlive() { return hp > 0; }

};

// 몬스터들 
class Goblin : public Monster {
public:
	Goblin() : Monster("고블린", 40, 8, 12) {}

	void skill(Player& p) override {
		int r = rand() % 2;
		if (r == 0) {
			std::cout << "단검 찌르기! ";
			p.takedamge(atk, name);
		}
		if (r == 1) {
			std::cout << "단검 던지기! ";
			p.takedamge(atk + 5, name);
		}
	}
};

// 1층 보스
class GoblinKing : public Monster {
public:
	GoblinKing() : Monster("고블린 왕", 100, 9, 50) {}

	void skill(Player& p) override {
		int r = rand() % 3;
		if (r == 0) {
			std::cout << "철퇴 휘두르기! ";
			p.takedamge(atk, name);
		}
		else if (r == 1) {
			std::cout << "철퇴 내려찍기! ";
			p.takedamge(atk * 2, name);
		}
		else {
			std::cout << "고블린 왕이 고함을 지른다! 등골이 서늘해진다...\n";
			p.addFear(6);
		}
	}
};

// 2층
class Skeleton : public Monster {
public:
	Skeleton() : Monster("해골", 50, 10, 15) {}

	void skill(Player& p) override {
		int r = rand() % 2;
		if (r == 0) {
			std::cout << "녹슨 검 베기! ";
			p.takedamge(atk, name);
		}
		if (r == 1) {
			std::cout << "뼈 방패 밀치기! ";
			p.takedamge(atk - 3, name);
		}
	}
};

class Lich : public Monster { // 2층 보스
public:
	Lich() : Monster("리치", 130, 11, 80) {}

	void skill(Player& p) override {
		int r = rand() % 3;
		if (r == 0) {
			std::cout << "냉기 화살! ";
			p.takedamge(atk, name);
		}
		else if (r == 1) {
			std::cout << "냉기 폭발! ";
			p.takedamge(atk * 2, name);
		}
		else {
			std::cout << "리치가 당신의 이름을 부른다... 숨이 막힌다.\n";
			p.addFear(8);
		}
	}
};

// 3층
class Werewolf : public Monster {
public:
	Werewolf() : Monster("늑대인간", 60, 10, 25) {}

	void skill(Player& p) override {
		int r = rand() % 2;
		if (r == 0) {
			std::cout << "발톱 할퀴기! ";
			p.takedamge(atk, name);
		}
		if (r == 1) {
			std::cout << "물어뜯기! ";
			p.takedamge(atk + 4, name);
		}
	}
};
// 중간보스들
class Hobgoblin : public Monster {
public:
	Hobgoblin() : Monster("홉고블린", 70, 9, 30) {}

	void skill(Player& p) override {
		int r = rand() % 2;
		if (r == 0) {
			std::cout << "몽둥이 휘두르기! ";
			p.takedamge(atk + 5, name);
		}
		if (r == 1) {
			std::cout << "주먹질! ";
			p.takedamge(atk, name);
		}
	}
};

class SkeletonKnight : public Monster {
public:
	SkeletonKnight() : Monster("해골 기사", 100, 12, 45) {}

	void skill(Player& p) override {
		int r = rand() % 2;
		if (r == 0) {
			std::cout << "대검 베기! ";
			p.takedamge(atk + 8, name);
		}
		if (r == 1) {
			std::cout << "방패 밀치기! ";
			p.takedamge(atk, name);
		}
	}
};

class AlphaWolf : public Monster {
public:
	AlphaWolf() : Monster("우두머리 늑대", 110, 12, 60) {}

	void skill(Player& p) override {
		int r = rand() % 2;
		if (r == 0) {
			std::cout << "물어뜯기! ";
			p.takedamge(atk + 6, name);
		}
		if (r == 1) {
			std::cout << "달려들기! ";
			p.takedamge(atk, name);
		}
	}
};
class DarkLord : public Monster { // 최종보스
public:
	DarkLord() : Monster("어둠의 군주", 180, 13, 150) {}

	void skill(Player& p) override {
		int r = rand() % 3;
		if (r == 0) {
			std::cout << "암흑 검! ";
			p.takedamge(atk, name);
		}
		else if (r == 1) {
			std::cout << "파멸의 일격! ";
			p.takedamge(atk * 2, name);
		}
		else {
			std::cout << "어둠의 군주가 포효한다! 공포가 엄습한다...\n";
			p.addFear(10);
		}
	}
};

// 1층 몹 추가
class Slime : public Monster {
public:
	Slime() : Monster("슬라임", 45, 6, 10) {}

	void skill(Player& p) override {
		int r = rand() % 2;
		if (r == 0) {
			std::cout << "몸통 박치기! ";
			p.takedamge(atk, name);
		}
		if (r == 1) {
			std::cout << "산성 점액 뱉기! ";
			p.takedamge(atk + 3, name);
		}
	}
};

class Bat : public Monster {
public:
	Bat() : Monster("박쥐", 25, 7, 8) {}

	void skill(Player& p) override {
		int r = rand() % 2;
		if (r == 0) {
			std::cout << "물어뜯기! ";
			p.takedamge(atk, name);
		}
		if (r == 1) {
			std::cout << "초음파! 귀가 먹먹하다... ";
			p.addFear(3);
			p.takedamge(atk - 3, name);
		}
	}
};

// 2층 몹 추가
class Ghost : public Monster {
public:
	Ghost() : Monster("유령", 40, 8, 15) {}

	void skill(Player& p) override {
		int r = rand() % 2;
		if (r == 0) {
			std::cout << "차가운 손길! ";
			p.takedamge(atk, name);
		}
		if (r == 1) {
			std::cout << "유령이 귓가에 속삭인다... 등골이 서늘해진다.\n";
			p.addFear(6);
		}
	}
};

class Ghoul : public Monster {
public:
	Ghoul() : Monster("구울", 55, 10, 18) {}

	void skill(Player& p) override {
		int r = rand() % 2;
		if (r == 0) {
			std::cout << "할퀴기! ";
			p.takedamge(atk, name);
		}
		if (r == 1) {
			std::cout << "썩은 이빨로 물기! ";
			p.takedamge(atk + 4, name);
		}
	}
};

// 3층 몹 추가
class Spider : public Monster {
public:
	Spider() : Monster("독거미", 55, 10, 25) {}

	void skill(Player& p) override {
		int r = rand() % 2;
		if (r == 0) {
			std::cout << "다리 찌르기! ";
			p.takedamge(atk, name);
		}
		if (r == 1) {
			std::cout << "독니! ";
			p.takedamge(atk + 5, name);
		}
	}
};

class Shadow : public Monster {
public:
	Shadow() : Monster("그림자", 50, 9, 25) {}

	void skill(Player& p) override {
		int r = rand() % 2;
		if (r == 0) {
			std::cout << "어둠의 손아귀! ";
			p.takedamge(atk, name);
		}
		if (r == 1) {
			std::cout << "그림자가 당신의 모습을 흉내 낸다... 소름이 돋는다.\n";
			p.addFear(8);
		}
	}
};

// 직업들 (Player 상속, skill1 skill2 override)
// 기사 (체력 높음)
class knight : public Player {
public:
	knight() : Player(120, 50, 12, 20) {}

	bool skill1(Monster& m) override { // 방패 강타
		if (mana < 10) {
			std::cout << "마나 부족!\n";
			return false;
		}
		mana -= 10;
		std::cout << "방패 강타!\n";
		m -= skillatk;
		return true;
	}

	bool skill2(Monster& m) override { // 회전 베기
		if (mana < 20) {
			std::cout << "마나 부족!\n";
			return false;
		}
		mana -= 20;
		std::cout << "회전 베기!\n";
		m -= skillatk * 2;
		return true;
	}
};

// 마법사 (스킬 쌤)
class mage : public Player {
public:
	mage() : Player(80, 100, 10, 35) {}

	bool skill1(Monster& m) override { // 화염구
		if (mana < 15) {
			std::cout << "마나 부족!\n";
			return false;
		}
		mana -= 15;
		std::cout << "화염구!\n";
		m -= skillatk;
		return true;
	}

	bool skill2(Monster& m) override { // 번개
		if (mana < 30) {
			std::cout << "마나 부족!\n";
			return false;
		}
		mana -= 30;
		std::cout << "번개!\n";
		m -= skillatk * 2;
		return true;
	}
};

// 궁수
class achar : public Player {
public:
	achar() : Player(100, 60, 15, 12) {}

	bool skill1(Monster& m) override { // 연속 사격 (2번 때림)
		if (mana < 12) {
			std::cout << "마나 부족!\n";
			return false;
		}
		mana -= 12;
		std::cout << "연속 사격!\n";
		m -= skillatk;
		m -= skillatk;
		return true;
	}

	bool skill2(Monster& m) override { // 저격
		if (mana < 20) {
			std::cout << "마나 부족!\n";
			return false;
		}
		mana -= 20;
		std::cout << "저격!\n";
		m -= skillatk * 3;
		return true;
	}
};

void attack(Player& p, Monster& m) {
	// 공포 50 넘으면 가끔 빗나감
	if (p.getFear() >= 50 && rand() % 100 < 30) {
		std::cout << "손이 떨려 공격이 빗나갔다...\n";
		return;
	}
	int dmg = p.getatk();
	if (rand() % 100 < 10 + p.getLuck()) { // 치명타 10% + 운
		dmg = dmg * 2;
		std::cout << "치명타! ";
	}
	std::cout << "공격!\n";
	m -= dmg;
}


class Stock {
	int* count; // 0 체력포션 1 마나포션 2 진정제

public:
	Stock() {
		count = new int[3];
		count[0] = 3;
		count[1] = 3;
		count[2] = 3;
	}

	
	~Stock() { delete[] count; }

	
	Stock(const Stock& o) {
		count = new int[3];
		count[0] = o.count[0];
		count[1] = o.count[1];
		count[2] = o.count[2];
	}


	Stock& operator=(const Stock& o) {
		count[0] = o.count[0];
		count[1] = o.count[1];
		count[2] = o.count[2];
		return *this;
	}

	int get(int i) { return count[i]; }

	bool take(int i) { // 품절이면 false
		if (count[i] == 0) return false;
		count[i]--;
		return true;
	}
};

void shop(Player& p, Stock& stock) {
	while (true) {
		std::cout << "1.체력 포션 15G (남은 " << stock.get(0) << "개)  "
			<< "2.마나 포션 15G (남은 " << stock.get(1) << "개)  "
			<< "3.진정제 20G (남은 " << stock.get(2) << "개)  0.나가기\n";
		int c;
		std::cin >> c;

		if (c == 0) break;
		if (c != 1 && c != 2 && c != 3) {
			std::cout << "잘못된 번호입니다.\n";
			continue;
		}
		int price = 15;
		if (c == 3) price = 20;
		if (p.getGold() < price) {
			std::cout << "돈이 부족합니다!\n";
			continue;
		}
		if (!stock.take(c - 1)) {
			std::cout << "품절입니다!\n";
			continue;
		}
		p.spendGold(price);
		if (c == 1) p.additem("체력 포션");
		if (c == 2) p.additem("마나 포션");
		if (c == 3) p.additem("진정제");
	}
}
// 맵
// 방 번호 : 0 없음, 1 전투, 2 상점, 3 횃불, 4 보스, 5 빈 방, 6 중간보스, 9 시작
const int SIZE = 7; // 7x7

int map[SIZE][SIZE];
int dist[SIZE][SIZE]; // 시작방이랑 거리
bool visited[SIZE][SIZE]; // 가본방
int px, py; // 내 위치

int dx[4] = { 0, 0, -1, 1 }; // 위 아래 왼 오
int dy[4] = { -1, 1, 0, 0 };

// 방 있는지 확인
bool hasRoom(int x, int y) {
	if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) return false;
	return map[y][x] != 0;
}

// 옆에 방 몇개 있는지
int countNeighbor(int x, int y) {
	int cnt = 0;
	for (int d = 0; d < 4; d++) {
		if (hasRoom(x + dx[d], y + dy[d])) cnt++;
	}
	return cnt;
}

// 랜덤맵 (아이작처럼)
void makeMap() {
	while (true) {
		// 초기화
		for (int y = 0; y < SIZE; y++) {
			for (int x = 0; x < SIZE; x++) {
				map[y][x] = 0;
				dist[y][x] = 0;
				visited[y][x] = false;
			}
		}

		// 시작방은 가운데
		int sx = SIZE / 2, sy = SIZE / 2;
		map[sy][sx] = 9;
		std::vector<int> rx;
		std::vector<int> ry;
		rx.push_back(sx);
		ry.push_back(sy);

		// 방 9개 될때까지 옆에 붙이기
		int tries = 0;
		while (rx.size() < 9 && tries < 1000) {
			tries++;
			int r = rand() % rx.size();
			int d = rand() % 4;
			int nx = rx[r] + dx[d];
			int ny = ry[r] + dy[d];

			if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE) continue;
			if (map[ny][nx] != 0) continue;
			if (countNeighbor(nx, ny) != 1) continue; // 이거 해야 가지처럼 됨

			map[ny][nx] = 1;
			dist[ny][nx] = dist[ry[r]][rx[r]] + 1;
			rx.push_back(nx);
			ry.push_back(ny);
		}

		// 막다른방 찾기
		std::vector<int> ex;
		std::vector<int> ey;
		for (int i = 1; i < rx.size(); i++) {
			if (countNeighbor(rx[i], ry[i]) == 1) {
				ex.push_back(rx[i]);
				ey.push_back(ry[i]);
			}
		}
		if (rx.size() < 9 || ex.size() < 3) continue; // 안되면 다시

		// 제일 먼곳 보스
		int far = 0;
		for (int i = 1; i < ex.size(); i++) {
			if (dist[ey[i]][ex[i]] > dist[ey[far]][ex[far]]) far = i;
		}
		int bx = ex[far], by = ey[far];
		if (dist[by][bx] < 3) continue; // 너무 가까우면 다시
		map[by][bx] = 4;

		// 보스 앞은 중간보스
		for (int d = 0; d < 4; d++) {
			int nx = bx + dx[d], ny = by + dy[d];
			if (hasRoom(nx, ny)) map[ny][nx] = 6;
		}

		// 상점 횃불
		int count = 0;
		for (int i = 0; i < ex.size(); i++) {
			if (i == far) continue;
			if (count == 0) map[ey[i]][ex[i]] = 2;
			if (count == 1) map[ey[i]][ex[i]] = 3;
			count++;
		}

		px = sx;
		py = sy;
		visited[py][px] = true;
		return;
	}
}
// 이동 (1위 2아래 3왼 4오)
bool move(int dir) {
	int nx = px;
	int ny = py;

	if (dir == 1) ny--;
	else if (dir == 2) ny++;
	else if (dir == 3) nx--;
	else if (dir == 4) nx++;

	// 방 없으면 못감
	if (!hasRoom(nx, ny)) {
		std::cout << "그쪽에는 문이 없다.\n";
		return false;
	}

	px = nx;
	py = ny;
	visited[py][px] = true;
	return true;
}
void printMap() {
	std::cout << "\n[지도]  @:나  !:몬스터  M:중간보스  B:보스  $:상점  T:횃불  ?:안 가 본 방\n";
	for (int y = 0; y < SIZE; y++) {
		for (int x = 0; x < SIZE; x++) {
			if (x == px && y == py) std::cout << "[@]";
			else if (map[y][x] == 4) std::cout << "[B]"; // 보스는 보이게
			else if (map[y][x] != 0 && !visited[y][x]) std::cout << "[?]";
			else if (map[y][x] == 1) std::cout << "[!]";
			else if (map[y][x] == 2) std::cout << "[$]";
			else if (map[y][x] == 3) std::cout << "[T]";
			else if (map[y][x] == 6) std::cout << "[M]";
			else std::cout << "[ ]";
		}
		std::cout << "\n";
	}
}
// 증강 (보스 잡으면 고르기)

// 1층 증강
void augment1(Player& p) {
	std::cout << "\n========== 증강 선택 ==========\n";
	std::cout << "1. 강인함 : 최대 체력 +30\n";
	std::cout << "2. 날카로움 : 공격력 +5\n";
	std::cout << "3. 강철 정신 : 공포 -30\n";

	int c;
	std::cin >> c;
	if (c == 1) p.addMaxHp(30);
	else if (c == 2) p.addAtk(5);
	else p.addFear(-30);
	std::cout << "증강을 얻었다!\n";
}

// 2층 증강
void augment2(Player& p) {
	std::cout << "\n========== 증강 선택 ==========\n";
	std::cout << "1. 마력 폭주 : 스킬 공격력 +10\n";
	std::cout << "2. 깊은 샘 : 최대 마나 +30\n";
	std::cout << "3. 행운아 : 운 +10\n";

	int c;
	std::cin >> c;
	if (c == 1) p.addSkillAtk(10);
	else if (c == 2) p.addMaxMana(30);
	else p.addLuck(10);
	std::cout << "증강을 얻었다!\n";
}

// 전투 (이기면 true)
bool battle(Player* p, Monster& m) {
	while (p->life() && m.isAlive()) {
		if (p->getFear() >= 100) {
			std::cout << "공포를 견디지 못하고 정신이 무너졌다...\n";
			return false;
		}
		p->printStatus();
		m.printStatus();

		std::cout << "1.공격  2.스킬1  3.스킬2  4.아이템\n";
		int c;
		std::cin >> c;

		if (c == 1) attack(*p, m);
		else if (c == 2) {
			if (!p->skill1(m)) continue; // 마나 없으면 다시
		}
		else if (c == 3) {
			if (!p->skill2(m)) continue;
		}
		else {
			p->useItem();
			continue;
		}
		if (m.isAlive()) m.skill(*p); // 몹 턴
		if (!m.isAlive()) {
			p->addGold(m.getGold());
			p->addMana(25); // 이기면 마나 회복 (마나 복사 버그때문에 여기로 옮김)
		}

	}
	return p->life();
}

int main() {
	srand(time(0));

	std::string a;
	std::cout << "이름을 정해주세요: ";
	std::cin >> a;
	


	int b;
	while (true) {
		std::cout << "직업 선택 (1.기사 2.마법사 3.궁수): ";
		std::cin >> b;
		if (b == 1) break;
		if (b == 2) break;
		if (b == 3) break;
		std::cout << "잘못된 번호입니다.\n";
	}

	Player* p = nullptr; //직업 
	if (b == 1) p = new knight();
	else if (b == 2) p = new mage();
	else if (b == 3) p = new achar();
	if (a == "rkdxodn") {
		p->addGold(123123); //달걀
	}

	// 스토리 시작
	std::cout << "\n어느 날부터 마을 사람들이 하나둘 사라지기 시작했다.\n";
	std::this_thread::sleep_for(std::chrono::seconds(1)); // 1초 쉬기
	std::cout << "사람들은 던전 깊은 곳에 사는 '어둠의 군주' 짓이라고 수군거렸다.\n";
	std::this_thread::sleep_for(std::chrono::seconds(1)); // 1초 쉬기
	std::cout << a << "은(는) 횃불 하나를 들고 던전으로 들어간다...\n\n";
	std::this_thread::sleep_for(std::chrono::seconds(1)); // 1초 쉬기
	std::cout << "이장 : 허허, 자네 처음 보는 얼굴이군.\n";
	std::this_thread::sleep_for(std::chrono::seconds(1)); // 1초 쉬기
	std::cout << "이장 : 마왕을 무찌르기 전에 포션 좀 받아 가게나...\n";
	std::this_thread::sleep_for(std::chrono::seconds(1)); // 1초 쉬기
	p->additem("체력 포션"); // 이장이 준 포션
	p->additem("마나 포션");


	Stock stock;

	for (int floor = 0; floor < 3; floor++) {
		std::cout << "\n===== " << floor + 1 << "층 =====\n";
		if (floor == 0) std::cout << "축축한 동굴. 어디선가 고블린들의 웃음소리가 들린다.\n";
		if (floor == 1) std::cout << "차가운 공기... 바닥에 오래된 뼈들이 굴러다닌다.\n";
		if (floor == 2) std::cout << "빛이 전혀 들지 않는다. 무언가가 나를 지켜보고 있다.\n";
		std::this_thread::sleep_for(std::chrono::seconds(1)); // 1초 쉬기
		makeMap();
		stock = Stock(); // 층마다 재고 다시 채움

		while (p->life() && p->getFear() < 100) {
			printMap();
			std::cout << "1.위 2.아래 3.왼쪽 4.오른쪽\n";
			int d;
			std::cin >> d;
			if (!move(d)) continue;
			p->addFear(3); // 걸으면 공포 올라감
			if (p->getFear() >= 100) {
				std::cout << "어둠을 더 이상 견딜 수 없다... 정신이 무너졌다.\n";
				break;
			}

			int room = map[py][px];
			if (room == 1) {
				Monster* m; 
				int r = rand() % 3;
				if (floor == 0) {
					if (r == 0) m = new Goblin();
					else if (r == 1) m = new Slime();
					else m = new Bat();
				}
				else if (floor == 1) {
					if (r == 0) m = new Skeleton();
					else if (r == 1) m = new Ghost();
					else m = new Ghoul();
				}
				else {
					if (r == 0) m = new Werewolf();
					else if (r == 1) m = new Spider();
					else m = new Shadow();
				}

				std::cout << "몬스터가 나타났다!\n";
				if (battle(p, *m)) map[py][px] = 5;
				delete m;
			}
			else if (room == 2) shop(*p, stock);
			else if (room == 3) {
				std::cout << "횃불이 타오르고 있다.\n";
				std::cout << "1.횃불에 잠깐 몸을 기댄다  0.나가기\n";
				int t;
				std::cin >> t;
				if (t == 1) {
					std::cout << "따뜻한 온기가 몸을 감싼다...\n";
					p->heal(30);
					p->addFear(-30);
					map[py][px] = 5; // 한번 쓰면 끝
				}
			}
			else if (room == 6) {
				Monster* mid;
				if (floor == 0) mid = new Hobgoblin();
				else if (floor == 1) mid = new SkeletonKnight();
				else mid = new AlphaWolf();

				std::cout << "중간보스가 나타났다!\n";
				if (battle(p, *mid)) {
					map[py][px] = 5;
					std::cout << "중간보스를 쓰러뜨리고 숨을 돌린다.\n";
					p->heal(30);
				}
				delete mid;
			}
			else if (room == 4) {
				Monster* boss;
				if (floor == 0) boss = new GoblinKing();
				else if (floor == 1) boss = new Lich();
				else boss = new DarkLord();

				std::cout << "보스가 나타났다!\n";
				if (floor == 0) std::cout << "고블린 왕: \"감히 내 동굴에 들어오다니!\"\n";
				if (floor == 1) std::cout << "리치: \"산 자가 여기까지 오다니... 너도 내 하인이 되어라.\"\n";
				if (floor == 2) std::cout << "어둠의 군주: \"마을 사람들처럼 너도 어둠에 삼켜져라.\"\n";
				std::this_thread::sleep_for(std::chrono::seconds(1)); // 1초 쉬기
				bool win = battle(p, *boss);
				delete boss;
				if (win) {
					std::cout << "보스를 쓰러뜨렸다!\n";
					if (floor == 0) augment1(*p);
					if (floor == 1) augment2(*p);
					break;
				}
			}
		}

		if (!p->life() || p->getFear() >= 100) break;

		if (floor < 2) {
			std::cout << "계단을 내려가며 잠시 숨을 고른다...\n";
			p->heal(50);
			p->addFear(-15);
		}
	}

	if (p->life() && p->getFear() < 100) {
		std::cout << "\n어둠의 군주가 쓰러지자 던전에 빛이 스며든다.\n";
		std::this_thread::sleep_for(std::chrono::seconds(1)); // 1초 쉬기
		std::cout << "갇혀 있던 마을 사람들이 하나둘 밖으로 걸어 나온다.\n";
		std::this_thread::sleep_for(std::chrono::seconds(1)); // 1초 쉬기
		std::cout << a << "은(는) 마을의 영웅이 되었다.\n";
		std::this_thread::sleep_for(std::chrono::seconds(1)); // 1초 쉬기
		std::cout << "던전 클리어!\n";
	}
	else {
		std::cout << "\n" << a << "의 횃불이 꺼졌다...\n";
		std::this_thread::sleep_for(std::chrono::seconds(1)); // 1초 쉬기
		std::cout << "던전은 또 한 명의 모험가를 삼켰다.\n";
		std::this_thread::sleep_for(std::chrono::seconds(1)); // 1초 쉬기
		std::cout << "GAME OVER\n";
	}
	delete p;
}