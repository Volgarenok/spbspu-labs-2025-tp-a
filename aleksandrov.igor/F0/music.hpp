#ifndef MUSIC_HPP
#define MUSIC_HPP

#include <map>
#include <deque>
#include <string>

namespace aleksandrov
{
  enum class NoteType;
  enum class MusicalElementType;

  struct Note;
  struct Interval;
  struct Chord;
  class MusicalElement;

  using Sequence = std::deque< MusicalElement >;
}

enum class aleksandrov::MusicalElementType
{
  Note,
  Interval,
  Chord,
  None
};

struct aleksandrov::Note
{
  char letter;
  char accidental;
  char pitch;

  std::string toString() const;
  short toSemitones() const;
};

struct aleksandrov::Interval
{
  Note first;
  Note second;

  void inverseUp();
  void inverseDown();
};

struct aleksandrov::Chord
{
  struct NoteComparator
  {
    bool operator()(const Note& lhs, const Note& rhs) const
    {
      return lhs.toSemitones() < rhs.toSemitones();
    }
  };
  std::map< Note, size_t, NoteComparator > notes;

  void inverseUp();
  void inverseDown();
};

class aleksandrov::MusicalElement
{
public:
  MusicalElement();
  MusicalElement(const MusicalElement&);
  explicit MusicalElement(const Note&);
  explicit MusicalElement(const Interval&);
  explicit MusicalElement(const Chord&);
  ~MusicalElement() noexcept;

  MusicalElement& operator=(const MusicalElement&);

  MusicalElementType getType() const noexcept;

  Note& note();
  const Note& note() const;
  Interval& interval();
  const Interval& interval() const;
  Chord& chord();
  const Chord& chord() const;

  bool isNote() const noexcept;
  bool isInterval() const noexcept;
  bool isChord() const noexcept;
  bool isNone() const noexcept;

  void inverseUp();
  void inverseDown();

private:
  MusicalElementType type_;
  union
  {
    Note note_;
    Interval interval_;
    Chord chord_;
  };
};

#endif

