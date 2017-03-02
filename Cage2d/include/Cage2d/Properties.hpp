#ifndef PROPERTIES_HPP_INCLUDED
#define PROPERTIES_HPP_INCLUDED

/*
** C# style properties in C++
** (C) 2010 Arkanosis
** jroquet@arkanosis.net
**
** Example code released under the MIT license
** http://www.opensource.org/licenses/mit-license.php
**
** This is an original portable version of
**  http://www.codeproject.com/KB/cpp/properties.aspx
** which only worked under MSVC
*/

/*
** This is the C#-like version, for the Python-like version, see:
** http://gist.github.com/632240
*/


/************************************************************
** This is where the magic goes
*************************************************************/

template <typename ClassT, typename TypeT, TypeT (ClassT::*Getter)() const>
class _Get
{
public:
  explicit _Get(ClassT const* self)
    : _this(self) {
  }
  inline operator TypeT() const
  {
    return (_this->*Getter)();
  }
  TypeT operator=(TypeT value) const = delete;
  _Get<ClassT, TypeT, Getter>& operator=(const _Get<ClassT, TypeT, Getter> &value) const = delete;

private:
  ClassT const* _this;
};
template <typename ClassT, typename TypeT, void (ClassT::*Setter)(const TypeT&)>
class _Set
{
public:
  explicit _Set(ClassT* self)
    : _this(self) {
  }
  inline const TypeT& operator=(const TypeT& value) const
  {
    (_this->*Setter)(value);
    return value;
  }
  operator TypeT() = delete;
private:
  ClassT* _this;
};
template <typename ClassT, typename TypeT, TypeT (ClassT::*Getter)() const, void (ClassT::*Setter)(const TypeT&)>
class _GetSet
{
public:
  explicit _GetSet(ClassT* self)
    : _this(self) {
  }
  inline operator TypeT() const
  {
    return (_this->*Getter)();
  }
  inline const TypeT& operator=(const TypeT& value) const
  {
    (_this->*Setter)(value);
    return value;
  }
private:
  ClassT* _this;
};

#define GET(CLASS, TYPE, NAME, CODE)         \
  inline TYPE get##NAME() const CODE         \
  _Get<CLASS, TYPE, &CLASS::get##NAME> NAME

#define SET(CLASS, TYPE, NAME, CODE)         \
  inline void set##NAME(const TYPE& value) CODE     \
  _Set<CLASS, TYPE, &CLASS::set##NAME> NAME

#define GETSET(CLASS, TYPE, NAME, GET, SET)  \
  inline TYPE get##NAME() const GET          \
  inline void set##NAME(const TYPE& value) SET      \
  _GetSet<CLASS, TYPE, &CLASS::get##NAME, &CLASS::set##NAME> NAME

#endif // PROPERTIES_HPP_INCLUDED
