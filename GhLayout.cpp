//
//  GhLayout.cpp
//  testlayout
//
//  Created by guolihui on 15/12/18.
//  Copyright © 2015 guolihuiguolihui. All rights reserved.
//

#include "GhLayout.h"
#include <stdexcept>

using namespace ghlayout;

namespace  {
    int calcBitCount(unsigned int n)
    {
        unsigned int c =0 ;
        for (c =0; n; ++c)
        {
            n &= (n -1);
        }
        return c ;
    }
}

GhNode::GhNode(cocos2d::Node* node) {
    _node = node;
}

#define DEF_ATTRIBUTE_METHOD_RVALUE(methodName, AttributeName)\
GhAttribute GhNode::gh_##methodName() const {\
    return GhAttribute(_node, AttributeName);\
}

#define DEF_ATTRIBUTE_METHOD_LVALUE(methodName, AttributeName)\
GhNode& GhNode::methodName() {\
    _attribute |= AttributeName;\
    return *this;\
}

#define DEF_ATTRIBUTE_METHOD(methodName, AttributeName)\
    DEF_ATTRIBUTE_METHOD_RVALUE(methodName, AttributeName) \
    DEF_ATTRIBUTE_METHOD_LVALUE(methodName, AttributeName)

DEF_ATTRIBUTE_METHOD(left, ATTRIBUTE_LEFT)
DEF_ATTRIBUTE_METHOD(right, ATTRIBUTE_RIGHT)
DEF_ATTRIBUTE_METHOD(top, ATTRIBUTE_TOP)
DEF_ATTRIBUTE_METHOD(bottom, ATTRIBUTE_BOTTOM)
DEF_ATTRIBUTE_METHOD_RVALUE(height, ATTRIBUTE_HEIGHT)
DEF_ATTRIBUTE_METHOD_RVALUE(width, ATTRIBUTE_WIDTH)
DEF_ATTRIBUTE_METHOD(centerX, ATTRIBUTE_CENTERX)
DEF_ATTRIBUTE_METHOD(centerY, ATTRIBUTE_CENTERY)

GhNode& GhNode::with() {
    return *this;
}


#define DEF_OPERATOR_METHOD_RVALUE(methodName, OperatorType)\
GhAttribute GhAttribute::methodName(float value) const {\
    GhAttribute node = *this;\
    OperatorItem item;\
    item.operatorType = OperatorType;\
    item.operatorArg = value;\
    node._operatorItems.push_back(item);\
    return node;\
}

#define DEF_OPERATOR_METHOD_LVALUE(methodName, OperatorType)\
GhNode& GhNode::methodName(float value) {\
    GhAttribute attr;\
    GhAttribute::OperatorItem item;\
    item.operatorType = OperatorType;\
    item.operatorArg = value;\
    attr._operatorItems.push_back(item);\
    *this = attr; \
    return *this;\
}

DEF_OPERATOR_METHOD_RVALUE(offset, OPERATOR_OFFSET)
DEF_OPERATOR_METHOD_RVALUE(multipliedBy, OPERATOR_MULTIPLIEDBY)
DEF_OPERATOR_METHOD_RVALUE(dividedBy, OPERATOR_DEVIDEDBY)

DEF_OPERATOR_METHOD_LVALUE(operator+=, OPERATOR_OFFSET)
DEF_OPERATOR_METHOD_LVALUE(operator*=, OPERATOR_MULTIPLIEDBY)
DEF_OPERATOR_METHOD_LVALUE(operator/=, OPERATOR_DEVIDEDBY)

GhNode GhNode::operator+(float value) {
    GhNode node = *this;
    node += value;
    return node;
}

GhNode GhNode::operator-(float value) {
    return this->operator+(-value);
}

GhNode GhNode::operator*(float value) {
    GhNode node = *this;
    node *= value;
    return node;
}

GhNode GhNode::operator/(float value) {
    return this->operator*(1.0f/value);
}

GhAttribute GhNode::offset(float offset) const {
    if(calcBitCount(_attribute) > 1){
        throw std::runtime_error("GhNode().offset() must has less than 2 attribute");
    }
    return GhAttribute(_node, (enAttributeType)_attribute).offset(offset);
}
GhAttribute GhNode::multipliedBy(float value) const {
    if(calcBitCount(_attribute) > 1){
        throw std::runtime_error("GhNode().multipliedBy() must has less than 2 attribute");
    }
    return GhAttribute(_node, (enAttributeType)_attribute).multipliedBy(value);
}
GhAttribute GhNode::dividedBy(float value) const {
    if(calcBitCount(_attribute) > 1){
        throw std::runtime_error("GhNode().dividedBy() must has less than 2 attribute");
    }
    return GhAttribute(_node, (enAttributeType)_attribute).dividedBy(value);
}

GhNode& GhNode::operator-=(float offset){
    return this->operator+=(-offset);
}

cocos2d::Size GhNode::calcCCNodeContentSize(cocos2d::Node *pNode) {
    cocos2d::Size size = pNode->getContentSize();
    return size;
}
cocos2d::Size GhNode::calcCCNodeWorldContentSize(cocos2d::Node *pNode) {
    cocos2d::Size size = pNode->getContentSize();
    auto trasform = pNode->getNodeToWorldAffineTransform();
    size = cocos2d::SizeApplyAffineTransform(size, trasform);
    return size;
}

float GhNode::calcCCNodeAttributeValue(cocos2d::Node* pNode, enAttributeType attributeType) {
    switch (attributeType) {
        case ATTRIBUTE_LEFT:
        {
            float posX = pNode->getPositionX() - calcCCNodeContentSize(pNode).width * (pNode->isIgnoreAnchorPointForPosition()?0:pNode->getAnchorPoint().x)*pNode->getScaleX();
            posX = (pNode->getParent()?pNode->getParent():pNode)->convertToWorldSpace(cocos2d::Vec2(posX, 0)).x;
            return posX;
        }
            break;
        case ATTRIBUTE_RIGHT:
        {
            float posX = pNode->getPositionX() + calcCCNodeContentSize(pNode).width * (1-(pNode->isIgnoreAnchorPointForPosition()?0:pNode->getAnchorPoint().x))*pNode->getScaleX();
            posX = (pNode->getParent()?pNode->getParent():pNode)->convertToWorldSpace(cocos2d::Vec2(posX, 0)).x;
            return posX;
        }
            break;
        case ATTRIBUTE_TOP:
        {
            float posY = pNode->getPositionY() + calcCCNodeContentSize(pNode).height * (1-(pNode->isIgnoreAnchorPointForPosition()?0:pNode->getAnchorPoint().y))*pNode->getScaleY();
            posY = (pNode->getParent()?pNode->getParent():pNode)->convertToWorldSpace(cocos2d::Vec2(0, posY)).y;
            return posY;
        }
            break;
        case ATTRIBUTE_BOTTOM:
        {
            float posY = pNode->getPositionY() - calcCCNodeContentSize(pNode).height * (pNode->isIgnoreAnchorPointForPosition()?0:pNode->getAnchorPoint().y)*pNode->getScaleY();
            posY = (pNode->getParent()?pNode->getParent():pNode)->convertToWorldSpace(cocos2d::Vec2(0,posY)).y;
            return posY;
        }
            break;
        case ATTRIBUTE_WIDTH:
            return calcCCNodeContentSize(pNode).width;
            break;
        case ATTRIBUTE_HEIGHT:
            return calcCCNodeContentSize(pNode).height;
            break;
        case ATTRIBUTE_CENTERX:
        {
            float posX = pNode->getPositionX() + calcCCNodeContentSize(pNode).width * (0.5-(pNode->isIgnoreAnchorPointForPosition()?0:pNode->getAnchorPoint().x))*pNode->getScaleX();
            posX = (pNode->getParent()?pNode->getParent():pNode)->convertToWorldSpace(cocos2d::Vec2(posX,0)).x;
            return posX;
        }
            break;
        case ATTRIBUTE_CENTERY:
        {
            float posY = pNode->getPositionY() + calcCCNodeContentSize(pNode).height * (0.5-(pNode->isIgnoreAnchorPointForPosition()?0:pNode->getAnchorPoint().y))*pNode->getScaleY();
            posY = (pNode->getParent()?pNode->getParent():pNode)->convertToWorldSpace(cocos2d::Vec2(0,posY)).y;
            return posY;
        }
            break;
        default:
            break;
    }
    return 0;
}

float GhNode::calcCCNodeAttributeValueWithOperate(cocos2d::Node* pNode, enAttributeType attributeType, const std::vector <GhAttribute:: OperatorItem>& operatorItems) {
    float attributeValue = GhNode::calcCCNodeAttributeValue(pNode, attributeType);
    
    float rightValue = attributeValue;
    for (const GhAttribute::OperatorItem& item : operatorItems){
        switch (item.operatorType) {
            case OPERATOR_OFFSET:
                rightValue += item.operatorArg;
                break;
            case OPERATOR_MULTIPLIEDBY:
                rightValue *= item.operatorArg;
                break;
            case OPERATOR_DEVIDEDBY:
                rightValue /= item.operatorArg;
                break;
            default:
                break;
        }
    }
    
    return rightValue;
}

GhNode& GhNode::operator=(const GhNode &node){
    if (_attribute == ATTRIBUTE_NONE){
        throw std::runtime_error("GhNode().operator=(const GhNode &) must has attribute");
    }
    if (!node._node){
        throw std::runtime_error("GhNode().operator=(const GhNode &) must has cocos2d::Node");
    }
    
    int count = calcBitCount(node._attribute);
    if (count > 1) {
        throw std::runtime_error("GhNode().operator=(const GhNode &) must has less than 2 attribute");
    }
    if (count == 0){
        GhAttribute attr(node._node, ATTRIBUTE_NONE);
        return this->operator=(attr);
    }
    static const int testTbl[] = {
        ATTRIBUTE_LEFT,
        ATTRIBUTE_RIGHT,
        ATTRIBUTE_TOP,
        ATTRIBUTE_BOTTOM,
        ATTRIBUTE_WIDTH,
        ATTRIBUTE_HEIGHT,
        ATTRIBUTE_CENTERX,
        ATTRIBUTE_CENTERY,
    };
    for (int i = 0; i < sizeof(testTbl)/sizeof(testTbl[0]); ++i){
        if (testTbl[i] & node._attribute)
            return this->operator=(GhAttribute(node._node, (enAttributeType)testTbl[i]));
    }
    return *this;
}

GhNode& GhNode::operator=(float value) {
    float rightValue = value;
    if (_attribute & ATTRIBUTE_LEFT)
    {
        float posX = rightValue + calcCCNodeContentSize(_node).width * (_node->isIgnoreAnchorPointForPosition()?0:_node->getAnchorPoint().x);
        _node->setPositionX(posX);
    }
    if (_attribute & ATTRIBUTE_RIGHT)
    {
        float posX = rightValue - calcCCNodeContentSize(_node).width * (1- (_node->isIgnoreAnchorPointForPosition()?0:_node->getAnchorPoint().x));
        _node->setPositionX(posX);
    }
    if (_attribute & ATTRIBUTE_TOP)
    {
        float posY = rightValue - calcCCNodeContentSize(_node).height * (1-(_node->isIgnoreAnchorPointForPosition()?0:_node->getAnchorPoint().y));
        _node->setPositionY(posY);
    }
    if (_attribute & ATTRIBUTE_BOTTOM)
    {
        float posY = rightValue + calcCCNodeContentSize(_node).height * (_node->isIgnoreAnchorPointForPosition()?0:_node->getAnchorPoint().y);
        _node->setPositionY(posY);
    }
    if (_attribute & ATTRIBUTE_CENTERX)
    {
        float posX = rightValue - calcCCNodeContentSize(_node).width*(0.5-(_node->isIgnoreAnchorPointForPosition()?0:_node->getAnchorPoint().x));
        _node->setPositionX(posX);
    }
    if (_attribute & ATTRIBUTE_CENTERY)
    {
        float posY = rightValue - calcCCNodeContentSize(_node).height * (0.5-(_node->isIgnoreAnchorPointForPosition()?:_node->getAnchorPoint().y));
        _node->setPositionY(posY);
    }
    return *this;
}

GhNode& GhNode::operator=(const GhAttribute &node){
    if (calcBitCount(node._attribute) > 1) {
        throw std::runtime_error("GhNode().operator=(const GhAttribute &) must has less than 2 attribute");
    }
    cocos2d::Node* pSrcNode = node._node?node._node:_node;

    if (_attribute & ATTRIBUTE_LEFT)
    {
        float rightValue = GhNode::calcCCNodeAttributeValueWithOperate(pSrcNode, node._attribute!=ATTRIBUTE_NONE?node._attribute:ATTRIBUTE_LEFT, node._operatorItems);
        float posX = rightValue + calcCCNodeWorldContentSize(_node).width * (_node->isIgnoreAnchorPointForPosition()?0:_node->getAnchorPoint().x);
        posX = _node->getParent()->convertToNodeSpace(cocos2d::Vec2(posX, 0)).x;
        _node->setPositionX(posX);
    }
    if (_attribute & ATTRIBUTE_RIGHT)
    {
        float rightValue = GhNode::calcCCNodeAttributeValueWithOperate(pSrcNode, node._attribute!=ATTRIBUTE_NONE?node._attribute:ATTRIBUTE_RIGHT, node._operatorItems);
        float posX = rightValue - calcCCNodeWorldContentSize(_node).width * (1- (_node->isIgnoreAnchorPointForPosition()?0:_node->getAnchorPoint().x));
        posX = _node->getParent()->convertToNodeSpace(cocos2d::Vec2(posX, 0)).x;
        _node->setPositionX(posX);
    }
    if (_attribute & ATTRIBUTE_TOP)
    {
        float rightValue = GhNode::calcCCNodeAttributeValueWithOperate(pSrcNode, node._attribute!=ATTRIBUTE_NONE?node._attribute:ATTRIBUTE_TOP, node._operatorItems);
        float posY = rightValue - calcCCNodeWorldContentSize(_node).height * (1-(_node->isIgnoreAnchorPointForPosition()?0:_node->getAnchorPoint().y));
        posY = _node->getParent()->convertToNodeSpace(cocos2d::Vec2(0, posY)).y;
        _node->setPositionY(posY);
    }
    if (_attribute & ATTRIBUTE_BOTTOM)
    {
        float rightValue = GhNode::calcCCNodeAttributeValueWithOperate(pSrcNode, node._attribute!=ATTRIBUTE_NONE?node._attribute:ATTRIBUTE_BOTTOM, node._operatorItems);
        float posY = rightValue + calcCCNodeWorldContentSize(_node).height * (_node->isIgnoreAnchorPointForPosition()?0:_node->getAnchorPoint().y);
        posY = _node->getParent()->convertToNodeSpace(cocos2d::Vec2(0, posY)).y;
        _node->setPositionY(posY);
    }
    if (_attribute & ATTRIBUTE_CENTERX)
    {
        float rightValue = GhNode::calcCCNodeAttributeValueWithOperate(pSrcNode, node._attribute!=ATTRIBUTE_NONE?node._attribute:ATTRIBUTE_CENTERX, node._operatorItems);
        float posX = rightValue - calcCCNodeWorldContentSize(_node).width*(0.5-(_node->isIgnoreAnchorPointForPosition()?0:_node->getAnchorPoint().x));
        posX = _node->getParent()->convertToNodeSpace(cocos2d::Vec2(posX, 0)).x;
        _node->setPositionX(posX);
    }
    if (_attribute & ATTRIBUTE_CENTERY)
    {
        float rightValue = GhNode::calcCCNodeAttributeValueWithOperate(pSrcNode, node._attribute!=ATTRIBUTE_NONE?node._attribute:ATTRIBUTE_CENTERY, node._operatorItems);
        float posY = rightValue - calcCCNodeWorldContentSize(_node).height * (0.5-(_node->isIgnoreAnchorPointForPosition()?:_node->getAnchorPoint().y));
        posY = _node->getParent()->convertToNodeSpace(cocos2d::Vec2(0, posY)).y;
        _node->setPositionY(posY);
    }
    return *this;
}