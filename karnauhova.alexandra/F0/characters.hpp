#ifndef CHARACTERS_HPP
#define CHARACTERS_HPP

#include <string>

namespace karnauhova
{
  struct Character
  {
  public:
    Character();
    Character(std::string name, int hp, int atk1, int atk2);
    bool operator==(const Character& other) const;
    int position;
    int attack();
    void takePhilosophy(std::string philosophy);
    void takeStory(std::string story);
    void takeQuotes(std::string quote_win, std::string quote_lose);
    bool isLost();
    void damage(int size_damage);
    int volumeHp() const;
    int baseHp() const;
    std::string getName() const;
    std::string getStory() const;
    std::string getAtk() const;
    std::string getPhilosophy() const;
    std::string getWinQuote() const;
    std::string getLoseQuote() const;
    void aboutCharacter() const;
    void recoveryHp();

  private:
    std::string name_;
    int hp_;
    int total_hp_;
    int atk1_;
    int atk2_;
    std::string story_;
    std::string philosophy_;
    std::string quote_win_;
    std::string quote_lose_;

    int attackHead();
    int attackBody();
    int attackArm();
    int attackLeg();
  };
  std::istream& operator>>(std::istream& in, Character& pol);
  std::ostream& operator<<(std::ostream& out, const Character& character);
}
#endif
