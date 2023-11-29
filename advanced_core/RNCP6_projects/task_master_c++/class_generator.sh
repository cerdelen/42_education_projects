#!/bin/bash

mkdir -p includes
mkdir -p src

read -p "Enter Class Name: " ClassName

mkdir -p src/$ClassName
touch includes/$ClassName.hpp
touch src/$ClassName/$ClassName.cpp

echo "#ifndef "$ClassName"_DEFINED
#define "$ClassName"_DEFINED

class $ClassName
{
	public:
		$ClassName();
		$ClassName(const $ClassName &copy);
		$ClassName&		operator=(const $ClassName &copy);
		~$ClassName();
};

#endif // !$ClassName" >> includes/$ClassName.hpp


echo "#include <$ClassName.hpp>

$ClassName::$ClassName() {}
$ClassName::$ClassName(const $ClassName& copy) { *this = copy; }
$ClassName& $ClassName::operator=(const $ClassName& copy) {
    (void)copy;
    return (*this);
}
$ClassName::~$ClassName() {}" >> src/$ClassName/$ClassName.cpp
