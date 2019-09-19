// nodraw.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <windows.h> 
#include <chrono>
#include <thread> 
#include <ctime>

const int initialCarCount = 4;

const int carWidth = 25;
const int carHeight = 25;
const int offset = 6;
const int margin = 4;

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

struct Pos {
	int x;
	int y;

	Pos() { x = 0; y = 0; }
};

struct Size
{
	int width;
	int height;

	Size() { width = 0; height = 0; }
};

struct Rect
{
	Pos pos;
	Size size;

	Rect() = default;
	Rect(int w, int h) { pos.x = SCREEN_WIDTH; pos.y = SCREEN_HEIGHT; size.width = w; size.height = h; }
	Rect(int x, int y, int w, int h) { pos.x = x; pos.y = y; size.width = w; size.height = h; }

	void offsetX(int offset) {
		pos.x += offset;
	}

	void offsetY(int offset) {
		pos.y += offset;
	}

	void addH(int num) {
		size.height += num;
	}

	void addW(int num) {
		size.width += num;
	}

	bool rangeIntersect(int min0, int max0, int min1, int max1) {
		return max(min0, max0) >= min(min1, max1) && 
			   min(min0, max0) <= max(min1, max1);
	}

	bool intersects(const Rect& other) {
		int x0 = pos.x;
		int y0 = pos.y;
		int w0 = size.width;
		int h0 = size.height;

		int x1 = other.pos.x;
		int y1 = other.pos.y;
		int w1 = other.size.width;
		int h1 = other.size.height;

		return rangeIntersect(x0, x0 + w0, x1, x1 + w1) &&
			   rangeIntersect(y0, y0 + h0, y1, y1 + h1);
	}
};

enum Direction {
	UP,
	LEFT,
	RIGHT,
	DOWN
};

enum CarType {
	GAS,
	ELECTRO,
	HYBRID
};

struct Car
{
	Rect rect;
	Direction dir = Direction::UP;
	CarType type = CarType::GAS;

	int waitCounter = 0;
	int speed = 0;

	void wait() {
		waitCounter--;
	}

	virtual void move(std::vector<Car*> otherCars) {
		switch (dir) {
		case Direction::UP:
			rect.pos.y -= speed;
			// Respawns car if it leaves the screen
			if ((rect.pos.y <= (0 - rect.size.height)) && !crashingOnRespawn(otherCars)) {
				rect.pos.y = SCREEN_HEIGHT;
			}
			break;
		case Direction::DOWN:
			rect.pos.y += speed;
			if ((rect.pos.y >= SCREEN_HEIGHT + rect.size.height) && !crashingOnRespawn(otherCars)) {
				rect.pos.y = 0 - rect.size.height;
			}
			break;
		case Direction::LEFT:
			rect.pos.x -= speed;
			if ((rect.pos.x <= (0 - rect.size.width)) && !crashingOnRespawn(otherCars)) {
				rect.pos.x = SCREEN_WIDTH + rect.size.width;
			}
			break;
		case Direction::RIGHT:
			rect.pos.x += speed;
			if ((rect.pos.x >= SCREEN_WIDTH) && !crashingOnRespawn(otherCars)) {
				rect.pos.x = 0 - rect.size.width;
			}
			break;
		}
	}

	Rect nextPos() {
		switch (dir) {
			case Direction::UP:
				return Rect(rect.pos.x, rect.pos.y - speed, rect.size.width, rect.size.height);
			case Direction::DOWN:
				return Rect(rect.pos.x, rect.pos.y + speed, rect.size.width, rect.size.height);
			case Direction::RIGHT:
				return Rect(rect.pos.x + speed, rect.pos.y, rect.size.width, rect.size.height);
			case Direction::LEFT:
				return Rect(rect.pos.x - speed, rect.pos.y, rect.size.width, rect.size.height);
		}
	}

	Rect nextPos(int offset) {
		switch (dir) {
			case Direction::UP:
				return Rect(rect.pos.x, rect.pos.y - offset - speed, rect.size.width, rect.size.height);
			case Direction::DOWN:
				return Rect(rect.pos.x, rect.pos.y + offset + speed, rect.size.width, rect.size.height);
			case Direction::RIGHT:
				return Rect(rect.pos.x + offset + speed, rect.pos.y, rect.size.width, rect.size.height);
			case Direction::LEFT:
				return Rect(rect.pos.x - offset - speed, rect.pos.y, rect.size.width, rect.size.height);
		}
	}

	bool crashing(std::vector<Car*> otherCars) {
		for (auto otherCar : otherCars) {
			if (this == otherCar) {
				continue;
			}

			if (this->nextPos(offset).intersects(otherCar->rect)) {
				return true;
			}
		}

		return false;
	}

	bool crashingOnRespawn(std::vector<Car*> otherCars) {
		for (auto otherCar : otherCars) {
			if (this == otherCar) {
				continue;
			}

			switch (dir) {
			case Direction::UP:
				if ((rect.pos.y <= 0 - rect.size.height) &&
					Rect(rect.pos.x, SCREEN_HEIGHT, rect.size.width, rect.size.height).intersects(otherCar->rect))
					return true;
			case Direction::DOWN:
				if ((rect.pos.y >= SCREEN_HEIGHT) &&
					Rect(rect.pos.x, 0 - rect.size.height, rect.size.width, rect.size.height).intersects(otherCar->rect))
					return true;
			case Direction::LEFT:
				if ((rect.pos.x <= (0 - rect.size.width)) &&
					Rect(SCREEN_WIDTH + rect.size.width, rect.pos.y, rect.size.width, rect.size.height).intersects(otherCar->rect))
					return true;
			case Direction::RIGHT:
				if ((rect.pos.x >= SCREEN_WIDTH) &&
					Rect(0 - rect.size.width, rect.pos.y, rect.size.width, rect.size.height).intersects(otherCar->rect))
					return true;
			}
		}

		return false;
	}

	virtual int getFuel() = 0;
	virtual void refill(int count) = 0;
};

std::vector<Car*> cars;

struct GasCar : virtual Car
{
	int getFuel() override { return fuel; }
	void refill(int count) override { fuel += count; }
	void move(std::vector<Car*> otherCars) override { fuel--; Car::move(otherCars); }
	int fuel = 0;
};

struct ElectroCar : virtual Car
{
	int getFuel() override { return charge; }
	void refill(int count) override { charge += count; }
	void move(std::vector<Car*> otherCars) override { charge--; Car::move(otherCars); }
	int charge = 0;
};

struct HybridCar : GasCar, ElectroCar
{
	int getFuel() override { return charge + fuel; }
	void refill(int count) override { charge += count / 2; fuel += count / 2; }
	void move(std::vector<Car*> otherCars) override {
		if (rand() % 2 == 0)
			ElectroCar::move(otherCars);
		else
			GasCar::move(otherCars);
	}
};

Car* createCar() {
	Car* car;
	
	int carType = std::rand();
	if (carType % 3 == 0) {
		car = new GasCar();
		car->type = CarType::GAS;
		car->speed = 1;
	} else if (carType % 3 == 1) {
		car = new ElectroCar();
		car->type = CarType::ELECTRO;
		car->speed = 2;
	} else {
		car = new HybridCar();
		car->type = CarType::HYBRID;
		car->speed = 3;
	}

	car->rect = Rect(carWidth, carHeight);

	car->refill(100500);

	return car;
}

bool lettingThrough(Direction dir, Direction otherDir) {
	switch (dir) {
	case Direction::UP:
		if (otherDir == Direction::LEFT)
			return true;
		break;
	case Direction::DOWN:
		if (otherDir == Direction::RIGHT)
			return true;
		break;
	case Direction::RIGHT:
		if (otherDir == Direction::UP)
			return true;
		break;
	case Direction::LEFT:
		if (otherDir == Direction::DOWN)
			return true;
		break;
	}

	return false;
}

bool notTooLateToStop(Car *car, Car *otherCar) {
	switch (car->dir) {
	case Direction::UP:
		if (car->rect.pos.y <= otherCar->rect.pos.y + otherCar->rect.size.height) {
			return false;
		}
		break;
	case Direction::DOWN:
		if (car->rect.pos.y + car->rect.size.height >= otherCar->rect.pos.y) {
			return false;
		}
		break;
	case Direction::LEFT:
		if (car->rect.pos.x <= otherCar->rect.pos.x + otherCar->rect.size.width) {
			return false;
		}
		break;
	case Direction::RIGHT:
		if (car->rect.pos.x + car->rect.size.width >= otherCar->rect.pos.x) {
			return false;
		}
		break;
	}

	return true;
}

void decide(Car *car) {
	for (auto otherCar : cars) {
		if (car == otherCar) {
			continue;
		}

		Rect decisionRect;

		switch (car->dir) {
			case Direction::UP:
				decisionRect = car->nextPos(carHeight);
				decisionRect.addW(carWidth);
				break;
			case Direction::DOWN:
				decisionRect = car->nextPos(carHeight);
				decisionRect.offsetX(-carWidth);
				decisionRect.addW(carWidth);
				break;
			case Direction::LEFT:
				decisionRect = car->nextPos(carWidth);
				decisionRect.offsetY(-carHeight);
				decisionRect.addH(carHeight);
				break;
			case Direction::RIGHT:
				decisionRect = car->nextPos(carWidth);
				decisionRect.addH(carHeight);
				break;
		}

		if (decisionRect.intersects(otherCar->rect) 
			&& lettingThrough(car->dir, otherCar->dir)
			&& notTooLateToStop(car, otherCar)) {
			car->waitCounter = offset;
		}
	}
}

void spawnCar(Car *car) {
	int random = std::rand();

	if (random % 4 == 0) {
		car->dir = Direction::UP;
		car->rect.pos.x = SCREEN_WIDTH / 2 + carWidth / 2;
		car->rect.pos.y = SCREEN_HEIGHT;
	} else if (random % 4 == 1) {
		car->dir = Direction::DOWN;
		car->rect.pos.x = SCREEN_WIDTH / 2 - carWidth * 1.5;
		car->rect.pos.y = 0 - carHeight;
	} else if (random % 4 == 2) {
		car->dir = Direction::LEFT;
		car->rect.pos.x = SCREEN_WIDTH;
		car->rect.pos.y = SCREEN_HEIGHT / 2 + carWidth / 2;
	} else if (random % 4 == 3) {
		car->dir = Direction::RIGHT;
		car->rect.pos.x = 0 - carWidth;
		car->rect.pos.y = SCREEN_HEIGHT / 2 - carWidth * 1.5;
	}

	if (!car->crashing(cars)) {
		cars.push_back(car);
	}
}

void moveCars() {
	for (auto car : cars) {
		if (!car->crashing(cars)) {
			decide(car);
			if (car->waitCounter == 0) {
				car->move(cars);
			} else {
				car->wait();
			}
		}
	}
}

void drawCar(HWND console, Car *car) {
	Rect rect = car->rect;

	int w = rect.size.width;
	int h = rect.size.height;

	int x = rect.pos.x;
	int y = rect.pos.y;

	HDC hdc = GetDC(console);

	switch (car->type) {
	case CarType::GAS:
		SetDCPenColor(hdc, RGB(160, 100, 0));
		SetDCPenColor(hdc, RGB(160, 100, 0));
		break;
	case CarType::ELECTRO:
		SetDCPenColor(hdc, RGB(40, 30, 175));
		SetDCBrushColor(hdc, RGB(40, 30, 175));
		break;
	case CarType::HYBRID:
		SetDCPenColor(hdc, RGB(255, 30, 175));
		SetDCBrushColor(hdc, RGB(255, 30, 175));
		break;
	}

	if (car->waitCounter != 0) {
		SetDCPenColor(hdc, RGB(255, 45, 0));
		SetDCBrushColor(hdc, RGB(255, 45, 0));
	}

	SelectObject(hdc, GetStockObject(DC_PEN));
	SelectObject(hdc, GetStockObject(DC_BRUSH)); 

	Rectangle(hdc, x, y, x + w, y + h);
	 
	ReleaseDC(console, hdc);
}

void clear(HWND hwnd)
{
	// obtain a handle to the device context
	HDC hdc = GetDC(hwnd);
	SetDCBrushColor(hdc, RGB(0, 0, 0));
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	Rectangle(hdc, 0, 0, 1000, 500);
	 
	// release the DC
	ReleaseDC(hwnd, hdc);
}

void draw(std::vector<Car*> carsToDraw, HWND console) {
	clear(console);

	for (auto car : carsToDraw) {
		drawCar(console, car);
	}
}

void main_loop(HWND console) {
	while (true) {
		// Summons a car if spacebar is pressed
		if (GetKeyState(VK_SPACE) < 0) {
			spawnCar(createCar());
			std::chrono::milliseconds(70);
		}

		moveCars();

		draw(cars, console);

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

int main(int argc, char** argv) {
	HWND console = GetConsoleWindow();

	RECT ClientRect;
	GetClientRect(console, &ClientRect);

	SCREEN_WIDTH = ClientRect.right - ClientRect.left;
	SCREEN_HEIGHT = ClientRect.bottom - ClientRect.top;

	std::srand(std::time(nullptr));

	for (auto i = 0; i < initialCarCount; ++i) {
		spawnCar(createCar());
	}

	main_loop(console);

	return 0;
}
