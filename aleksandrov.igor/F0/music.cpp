#include "music.hpp"
#include <stdexcept>
#include <cassert>

namespace
{
  const std::map< char, short > notesSemitones = {
    { 'A', 0 },
    { 'B', 1 },
    { 'H', 2 },
    { 'C', 3 },
    { 'D', 5 },
    { 'E', 7 },
    { 'F', 8 },
    { 'G', 10 }
  };
}

std::string aleksandrov::Note::toString() const
{
  std::string str;
  str += letter;
  if (accidental != '\0')
  {
    str += accidental;
  }
  str += pitch;
  return str;
}

short aleksandrov::Note::toSemitones() const
{
  short semitones = notesSemitones.at(letter);
  if (accidental == '#')
  {
    ++semitones;
  }
  else if (accidental == 'b')
  {
    --semitones;
  }
  return (pitch - '0') * 12 + semitones;
}

void aleksandrov::Interval::inverseUp()
{
  if (first.pitch + 1 == '8' && first.letter != 'C')
  {
    throw std::logic_error("Inversing down note is out of range!");
  }
  else if (first.pitch > 56)
  {
    throw std::logic_error("Inversing down note is out of range!");
  }
  ++first.pitch;
  std::swap(first, second);
}

void aleksandrov::Interval::inverseDown()
{
  if (second.pitch - 1 == '0')
  {
    if (second.letter != 'H' && second.letter != 'A')
    {
      throw std::logic_error("Inversing down note is out of range!");
    }
  }
  else if (second.pitch < 48)
  {
    throw std::logic_error("Inversing down note is out of range!");
  }
  --second.pitch;
  std::swap(first, second);
}

void aleksandrov::Chord::inverseUp()
{
  if (notes.empty())
  {
    throw std::logic_error("Cannot inverse an empty chord!");
  }
  Note lowest = notes.begin()->first;
  Note inverted = lowest;
  ++inverted.pitch;
  if (inverted.pitch == '8' && inverted.letter != 'C')
  {
    throw std::logic_error("Inversing up lowest note is out of range!");
  }
  notes.erase(notes.begin());
  notes.emplace(inverted, inverted.toSemitones());
}

void aleksandrov::Chord::inverseDown()
{
  if (notes.empty())
  {
    throw std::logic_error("Cannot inverse an empty chord!");
  }
  Note highest = std::prev(notes.end())->first;
  Note inverted = highest;
  --inverted.pitch;
  if (inverted.pitch == '0')
  {
    if (inverted.letter != 'H' && inverted.letter != 'A')
    {
      throw std::logic_error("Inversing down note is out of range!");
    }
  }
  else if (inverted.pitch < '0')
  {
    throw std::logic_error("Inversing down note is out of range!");
  }
  notes.erase(std::prev(notes.end()));
  notes.emplace(inverted, inverted.toSemitones());
}

aleksandrov::MusicalElement::MusicalElement():
  type_(MusicalElementType::None)
{}

aleksandrov::MusicalElement::MusicalElement(const Note& note):
  type_(MusicalElementType::Note),
  note_(note)
{}

aleksandrov::MusicalElement::MusicalElement(const Interval& interval):
  type_(MusicalElementType::Interval),
  interval_(interval)
{}

aleksandrov::MusicalElement::MusicalElement(const Chord& chord):
  type_(MusicalElementType::Chord)
{
  new (std::addressof(chord_)) Chord(chord);
}

aleksandrov::MusicalElement::~MusicalElement() noexcept
{
  if (isChord())
  {
    chord_.~Chord();
  }
}

aleksandrov::MusicalElement::MusicalElement(const MusicalElement& other):
  type_(other.type_)
{
  switch (type_)
  {
  case MusicalElementType::Note:
    note_ = other.note_;
    break;
  case MusicalElementType::Interval:
    interval_ = other.interval_;
    break;
  case MusicalElementType::Chord:
    new (std::addressof(chord_)) Chord(other.chord_);
    break;
  default:
    break;
  }
}

aleksandrov::MusicalElement& aleksandrov::MusicalElement::operator=(const MusicalElement& rhs)
{
  if (isChord())
  {
    chord_.~Chord();
  }
  switch (rhs.type_)
  {
  case MusicalElementType::Note:
    note_ = rhs.note_;
    break;
  case MusicalElementType::Interval:
    interval_ = rhs.interval_;
    break;
  case MusicalElementType::Chord:
    new (std::addressof(chord_)) Chord(rhs.chord_);
    break;
  default:
    break;
  }
  type_ = rhs.type_;
  return *this;
}

aleksandrov::MusicalElementType aleksandrov::MusicalElement::getType() const noexcept
{
  return type_;
}

aleksandrov::Note& aleksandrov::MusicalElement::note()
{
  assert(isNote());
  return const_cast< Note& >(static_cast< const MusicalElement& >(*this).note());
}

const aleksandrov::Note& aleksandrov::MusicalElement::note() const
{
  assert(isNote());
  return note_;
}

aleksandrov::Interval& aleksandrov::MusicalElement::interval()
{
  assert(isInterval());
  return const_cast< Interval& >(static_cast< const MusicalElement& >(*this).interval());
}

const aleksandrov::Interval& aleksandrov::MusicalElement::interval() const
{
  assert(isInterval());
  return interval_;
}

aleksandrov::Chord& aleksandrov::MusicalElement::chord()
{
  assert(isChord());
  return const_cast< Chord& >(static_cast< const MusicalElement& >(*this).chord());
}

const aleksandrov::Chord& aleksandrov::MusicalElement::chord() const
{
  assert(isChord());
  return chord_;
}

bool aleksandrov::MusicalElement::isNote() const noexcept
{
  return type_ == MusicalElementType::Note;
}

bool aleksandrov::MusicalElement::isInterval() const noexcept
{
  return type_ == MusicalElementType::Interval;
}

bool aleksandrov::MusicalElement::isChord() const noexcept
{
  return type_ == MusicalElementType::Chord;
}

bool aleksandrov::MusicalElement::isNone() const noexcept
{
  return type_ == MusicalElementType::None;
}

void aleksandrov::MusicalElement::inverseUp()
{
  if (isNote())
  {
    throw std::logic_error("Cannot inverse a note!");
  }
  else if (isInterval())
  {
    interval_.inverseUp();
  }
  else if (isChord())
  {
    chord_.inverseUp();
  }
}

void aleksandrov::MusicalElement::inverseDown()
{
  if (isNote())
  {
    throw std::logic_error("Cannot inverse a note!");
  }
  else if (isInterval())
  {
    interval_.inverseDown();
  }
  else if (isChord())
  {
    chord_.inverseDown();
  }
}

