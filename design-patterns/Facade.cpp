#include <iostream>
#include <memory>

using namespace std;

class Light {
public:
  void on() { cout << "灯光打开" << endl; }
  void off() { cout << "灯光关闭" << endl; }
};

class Sound {
public:
  void on() { cout << "音响打开" << endl; }
  void off() { cout << "音响关闭" << endl; }
};

class Player {
public:
  void on() { cout << "播放器打开" << endl; }
  void off() { cout << "播放器关闭" << endl; }
};

class MovieTheater {
private:
  Light light_;
  Sound sound_;
  Player player_;

public:
  void WatchMovie() {
    cout << "开始观影" << endl;
    light_.on();
    sound_.on();
    player_.on();
  }
  void EndMovie() {
    cout << "结束观影" << endl;
    light_.off();
    sound_.off();
    player_.off();
  }
};

int main(){
    MovieTheater movie;
    movie.WatchMovie();
    movie.EndMovie();
}