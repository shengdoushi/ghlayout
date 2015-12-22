# 概述

一个简单的 cocos2d::Node 的布局DSL接口。

```cpp
using namespace ghlayout; // use namespace
cocos2d::Node* n1; cocos2d::Node* n2;

// n1.x = n2.x + 12
GhNode(n1).left() = GhNode(n2).left().offset(12); 
GhNode(n1).left() = GhNode(n2).offset(12);
GhNode(n1).left() = GhNode(n2) + 12;

// n1.x = (n2.x + n2.width) + 12
GhNode(n1).left() = GhNode(n2).right().offset(12);
GhNode(n1).left() = GhNode(n2).right().offset(12);

// n1.width = n2.width * 2
GhNode(n1).width() = GhNode(n2).width().multipliedBy(2);

// n1.width += 2
GhNode(n1).width() = GhNode(n1).width().offset(2);
GhNode(n1).width() += 2;
GhNode(n1).width() -= -2;

// n1.width *= 2
GhNode(n1).width() *= 2;
GhNode(n1).width() /= 0.5;

// n1.centerX = n2.centerX() * 0.5  即 n1 中心放置在n2内部的1/4处
GhNode(n1).centerX() = GhNode(n2).centerX().multipliedBy(0.5);
GhNode(n1).centerX() = GhNode(n2).multipliedBy(0.5);
GhNode(n1).centerX() = GhNode(n2) * 0.5;

```

