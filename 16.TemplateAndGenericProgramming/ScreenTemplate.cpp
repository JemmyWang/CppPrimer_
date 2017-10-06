#include "stdafx.h"
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

using pos = std::string::size_type;

template<pos, pos> class Screen;
template<pos H, pos W> istream& operator>> (istream&, Screen<H, W>&);
template<pos H, pos W> ostream& operator<< (ostream&, Screen<H, W>&);

template<pos H, pos W>
class Screen {
  friend istream& operator>><H, W>(istream&, Screen<H, W>&);
  friend ostream& operator<<<H, W>(ostream&, Screen<H, W>&);
public:
  Screen() = default;
  Screen(char c): contents(H * W, c) {}
  char get() { return contents[cursor]; }
  char get(pos r, pos c) { return contents[r * W + c]; }
  inline Screen& set(char c);
  inline Screen& set(pos r, pos c, char ch);
  inline Screen& move(pos r, pos c);
private:
  pos cursor = 0;
  string contents;
};

template<pos H, pos W> istream& operator>>(istream &is, Screen<H, W> &s) {
  string input;
  is >> input;
  for (char c : input) s.set(c);
  return is;
}

template<pos H, pos W> ostream& operator<<(ostream &os, Screen<H, W> &s) {
  for (pos r = 0; r < H; r++) {
    for (pos c = 0; c < W; c++) {
      os << s.get(r, c);
    }
    os << endl;
  }
  return os;
}

template<pos H, pos W> inline Screen<H, W>& Screen<H, W>::set(char ch) {
  contents[cursor++] = ch;
  cursor = std::min(cursor, H * W);
  return *this;
}

template<pos H, pos W> inline Screen<H, W>& Screen<H, W>::set(pos r, pos c, char ch) {
  contents[r * W + c] = ch;
  return *this;
}

template<pos H, pos W> inline Screen<H, W>& Screen<H, W>::move(pos r, pos c) {
  cursor = r * W + c;
  return *this;
}

//int main(void) {
//  Screen<5, 5> screen('x');
//  screen.set(2, 2, 'o');
//  screen.set(2, 3, 'J');
//  std::cout << screen << std::endl;
//  //screen.move(3, 3);
//  std::cout << "please input some characters as you like:";
//  std::cin >> screen;
//  std::cout << screen << std::endl;
//
//  return 0;
//}