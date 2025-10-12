#include "music-io-utils.hpp"
#include <cassert>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <stream-guard.hpp>

namespace
{
  bool isCorrectLetter(char l) noexcept
  {
    return 65 <= l && l <= 72;
  }

  bool isCorrectAccidental(char acc) noexcept
  {
    return acc == '#' || acc == 'b';
  }

  bool isCorrectPitch(char l, char p, char acc = '\0') noexcept
  {
    switch (p)
    {
    case '0':
      return l == 'H' || (l == 'A' && acc != 'b');
    case '8':
      return l == 'C' && !isCorrectAccidental(acc);
    default:
      return 49 <= p && p <= 55;
    }
  }
}

std::istream& aleksandrov::operator>>(std::istream& in, Note& note)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  Note temp;

  char l = '\0';
  in >> l;
  if (in && isCorrectLetter(l))
  {
    temp.letter = l;
  }
  else
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  char c = '\0';
  in >> std::noskipws >> c;
  if (in && isCorrectAccidental(c))
  {
    char p = '\0';
    in >> p;
    if (in && isCorrectPitch(l, p, c))
    {
      temp.accidental = c;
      temp.pitch = p;
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
  }
  else if (in && isCorrectPitch(l, c))
  {
    temp.pitch = c;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }

  if (in)
  {
    note = std::move(temp);
  }
  return in >> std::skipws;
}

std::istream& aleksandrov::operator>>(std::istream& in, Interval& interval)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  Note first;
  if (in >> first)
  {
    char c = '\0';
    in.get(c);
    if (in && c == '-')
    {
      Note second;
      if (in >> second)
      {
        if (first.toString() > second.toString())
        {
          std::swap(first, second);
        }
        interval.first = first;
        interval.second = second;
      }
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
  }
  return in;
}

std::istream& aleksandrov::operator>>(std::istream& in, Chord& chord)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  Chord temp;
  Note note;
  do
  {
    in >> note;
    if (!in)
    {
      break;
    }
    temp.notes.emplace(note, note.toSemitones());
  }
  while (in.peek() == '-' && in.get());

  if (in)
  {
    chord = std::move(temp);
  }
  return in;
}

std::istream& aleksandrov::operator>>(std::istream& in, MusicalElement& element)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  Chord temp;
  in >> temp;

  if (in || !temp.notes.empty())
  {
    if (temp.notes.size() == 1)
    {
      element = MusicalElement(temp.notes.begin()->first);
    }
    else if (temp.notes.size() == 2)
    {
      Note first = temp.notes.begin()->first;
      Note second = temp.notes.rbegin()->first;
      if (first.toString() > second.toString())
      {
        std::swap(first, second);
      }
      element = MusicalElement(Interval{ first, second });
    }
    else
    {
      element = MusicalElement(temp);
    }
  }
  else
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::ostream& aleksandrov::operator<<(std::ostream& out, const Note& note)
{
  StreamGuard guard(out);
  return out << note.toString();
}

std::ostream& aleksandrov::operator<<(std::ostream& out, const Interval& interval)
{
  StreamGuard guard(out);
  return out << interval.first << '-' << interval.second;
}

std::ostream& aleksandrov::operator<<(std::ostream& out, const Chord& chord)
{
  StreamGuard guard(out);
  const auto& notes = chord.notes;
  std::ostream_iterator< Note > outIt(out, "-");
  KeyReturner< Note, size_t > returner;
  std::transform(notes.begin(), std::prev(notes.end()), outIt, returner);
  return out << notes.rbegin()->first;
}

std::ostream& aleksandrov::operator<<(std::ostream& out, const MusicalElement& element)
{
  StreamGuard guard(out);
  switch (element.getType())
  {
  case MusicalElementType::Note:
    return out << element.note();
  case MusicalElementType::Interval:
    return out << element.interval();
  default:
    return out << element.chord();
  }
  assert(false && "Unsupported musical element!");
  return out;
}

