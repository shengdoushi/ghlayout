//
//  GhLayout.hpp
//  testlayout
//
//  Created by guolihui on 15/12/18.
//  Copyright © 2015 guolihuiguolihui. All rights reserved.
//
#pragma once
#include "cocos2d.h"
#include <vector>

namespace ghlayout {
    
    /**
     attribute type
     */
    enum enAttributeType {
        ATTRIBUTE_NONE = 0,
        ATTRIBUTE_LEFT = 1 << 1,
        ATTRIBUTE_RIGHT = 1 << 2,
        ATTRIBUTE_TOP = 1 << 3,
        ATTRIBUTE_BOTTOM = 1 << 4,
        ATTRIBUTE_WIDTH = 1 << 5,
        ATTRIBUTE_HEIGHT = 1 << 6,
        ATTRIBUTE_CENTERX = 1 << 7,
        ATTRIBUTE_CENTERY = 1 << 8,
        
    };
    /**
     operator type
     */
    enum enOperatorType {
        OPERATOR_NONE,
        OPERATOR_OFFSET,
        OPERATOR_MULTIPLIEDBY,
        OPERATOR_DEVIDEDBY,
    };
    
    class GhNode;
    
    /// attribute of node
    class GhAttribute {
    public:
        GhAttribute() {
            _attribute = enAttributeType::ATTRIBUTE_NONE;
        }
        GhAttribute(enAttributeType attr) {
            _node = nullptr;
            _attribute = attr;
        }
        GhAttribute(cocos2d::Node* pNode, enAttributeType attr) {
            _node = pNode;
            _attribute = attr;
        }
        GhAttribute(cocos2d::Node* pNode) {
            _node = pNode;
            _attribute = enAttributeType::ATTRIBUTE_NONE;
        }
        
        /**
         *  operator of attribute
         *
         *  @param value operator argument
         *
         *  @return new attribute (lazy)
         */
        GhAttribute offset(float value) const;
        GhAttribute multipliedBy(float value) const;
        GhAttribute dividedBy(float value) const;
        
        /**
         *  check whether has attribute
         *
         *  @return
         */
        bool hasAttribute() const {
            return _attribute == 0;
        }
        
        friend class GhNode;
    private:
        cocos2d::Node* _node = nullptr;
        enAttributeType _attribute = enAttributeType::ATTRIBUTE_NONE; // attribute
        struct OperatorItem {
            enOperatorType operatorType; // opeartor
            float operatorArg;   // operator arg
        };
        
        std::vector <OperatorItem> _operatorItems;
    };
    
    /// layout node
    class GhNode {
    public:
        /**
         *  node wrapper for cocos2d::Node
         */
        GhNode(cocos2d::Node* node);
        
        /**
         *  node's attribute (can modified)
         *
         *  @return attribute
         */
        GhNode& left();
        GhNode& right();
        GhNode& top();
        GhNode& bottom();
        GhNode& centerX();
        GhNode& centerY();
        
        /**
         *  readable helper
         *
         *  @return
         */
        GhNode& with();
        
        /**
         *  get attribute
         *
         *  @return attribute
         */
        GhAttribute gh_left() const;
        GhAttribute gh_right() const;
        GhAttribute gh_top() const;
        GhAttribute gh_bottom() const;
        GhAttribute gh_width() const;
        GhAttribute gh_height() const;
        GhAttribute gh_centerX() const;
        GhAttribute gh_centerY() const;
        
        /**
         *  operator of a node
         *
         *  @param value
         *
         *  @return new attribute (lazy)
         */
        GhAttribute offset(float value) const;
        GhAttribute multipliedBy(float value) const;
        GhAttribute dividedBy(float value) const;
        
        GhNode operator+(float value);
        GhNode operator-(float value);
        GhNode operator*(float value);
        GhNode operator/(float value);
        
        GhNode& operator += (float value);
        GhNode& operator -= (float value);
        GhNode& operator *= (float value);
        GhNode& operator /= (float value);
        GhNode& operator=(const GhAttribute& node);
        GhNode& operator=(const GhNode& node);
        GhNode& operator=(float value);
    protected:
        /**
         *  calculate a CCNode's attribute
         *
         *  @param pNode         cocos2d::Node
         *  @param attributeType attribute
         *
         *  @return value of attribute
         */
        static float calcCCNodeAttributeValue(cocos2d::Node* pNode, enAttributeType attributeType);
        static float calcCCNodeAttributeValueWithOperate(cocos2d::Node* pNode, enAttributeType attributeType, const std::vector <GhAttribute:: OperatorItem>& operatorItems);
        
        /**
         *  calculate a CCNode's content size
         *
         *  @param pNode CCNode
         *
         *  @return [contentSize.width, contentSize.height]
         */
        static cocos2d::Size calcCCNodeWorldContentSize(cocos2d::Node* pNode);
        static cocos2d::Size calcCCNodeContentSize(cocos2d::Node* pNode);
    private:
        /**
         *  cocos2d::Node
         */
        cocos2d::Node* _node;
        
        /**
         * operator
         */
        int _attribute = ATTRIBUTE_NONE;
    };
} // namespace ghlayout