//
//  MYtextfieldttf.cpp
//  GameSanGuo
//
//  Created by kanata on 14-1-10.
//
//

#include "MYtextfieldttf.h"
#include "SGNotificationCenter.h"
#include "GameConfig.h"
#include "SGMainManager.h"
#include "Utils.h"
#include "SGShowString.h"
MYtextfieldttf::MYtextfieldttf()
{
    m_positionPos = CCPointZero;
    m_nLength = 0;
    setKType(0);
    setclose(0);
}

MYtextfieldttf::~MYtextfieldttf()
{
    detachWithIME();
}

MYtextfieldttf * MYtextfieldttf::createWithPlaceHolder(CCPoint position,const char *placeholder, const CCSize& dimensions, CCTextAlignment alignment, const char *fontName, float fontSize)
{
    MYtextfieldttf *text = new MYtextfieldttf();
    text->m_positionPos = position;
    
    if(text && text->initWithPlaceHolder(placeholder, dimensions, alignment, fontName, fontSize))
    {
        text->viewDidLoad();
        text->autorelease();
        return text;
    }
    
    CC_SAFE_DELETE(text);
    return NULL;
}

#pragma mark - CCNode
void MYtextfieldttf::onEnter()
{
     CCNode::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, false);
}

void MYtextfieldttf::onExit()
{
    
     CCNode::onExit();
     setclose(1);
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    
   
//    if(getclose()==1)
//    {
//        detachWithIME();
//    }
}

#pragma mark - CCTouchDelegate

bool MYtextfieldttf::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint pt = pTouch->getLocationInView();
    pt = CCDirector::sharedDirector()->convertToGL(pt);
    
    m_positionPos = this->getPosition();
    
    CCRect rect = CCRectMake(m_positionPos.x-m_tDimensions.width/2 ,m_positionPos.y-m_tDimensions.height/2, m_tDimensions.width, m_tDimensions.height);
    
    if(rect.containsPoint(pt))
    {
        attachWithIME();
        return true;
    }
    
    return false;
}

void MYtextfieldttf::viewDidLoad()
{
    setString("");
    setPosition(m_positionPos);
}

void MYtextfieldttf::insertText(const char * text, int len)
{
    if (getKType() > 0) {
        if (GameConfig::isContainsEmoji(text)) {
            text = "";
        }
    }
    
    //判断如果超过最大数了，那么就不让加进去了
    //char buffer[1024]={};
    //strcpy(buffer, m_pInputText->c_str());
    //if(characterCount(buffer) >= m_nLength)
    //{
    
    //已经输入的总长度
    int existTextLen = getUtf8Length(*m_pInputText);
    //本次输入的长度
    int textInputLen = getUtf8Length(std::string(text) );
    if(existTextLen  ==  m_nLength)
    {
        SGMainManager::shareMain()->showMessage( str_ChatInputBox_str7 );
        if('\n' == *text)
        {
            detachWithIME();
        }
        return;
    }
    else if(existTextLen + textInputLen  > m_nLength)
    {
        SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_ChatInputBox_str5 , m_nLength - existTextLen )->getCString() );
        if('\n' == *text)
        {
            detachWithIME();
        }
        return;
    }
    CCTextFieldTTF::insertText(text, len);
}

//计算字符串长度
int MYtextfieldttf::characterCount(char *str)
{
    int chineseLength = 0;  //中文字符个数
    int englishLength = 0;  //英文字符个数
    int otherLength = 0;    //符号个数
    int index = 0;          //计数器
    
    while (str[index] != '\0') {
        
        //各种特殊符号
        //        if(str[index] == ' ' || str[index] == '\t' ||
        //           str[index] == '\r' || str[index] == '\n')
        //        {
        //            return -1;
        //        }
        
        if(('A' <= str[index] && 'Z' >= str[index]) || ('a' <= str[index] && 'z' >= str[index])){
            englishLength++;
        }else if(str[index] < 0){//这个判断暂定 ascii
            chineseLength++;
        }else{
            otherLength++;
        }
        
        index ++;
    }
    
    int length = englishLength + chineseLength/3 + otherLength;
    
    return length;
}

void MYtextfieldttf::keyboardWillShow(CCIMEKeyboardNotificationInfo& info)
{
    //    // CCLOG("CCEditBox::keyboardWillShow");
    //    CCRect rectTracked = getRect(this);
    //	// some adjustment for margin between the keyboard and the edit box.
    //	rectTracked.origin.y -= 4;
    //
    //    // if the keyboard area doesn't intersect with the tracking node area, nothing needs to be done.
    //    if (!rectTracked.intersectsRect(info.end))
    //    {
    //        CCLOG("needn't to adjust view layout.");
    //        return;
    //    }
    //
    
    
//    if (getParent() != NULL ) {
//        CCPoint moveBypos = ccpAdd(ccp(info.end.origin.x - info.begin.origin.x,info.end.origin.y - info.begin.origin.y), ccp(info.end.size.width - info.begin.size.width,info.end.size.height - info.begin.size.height));
//        getParent()->runAction(CCMoveBy::create(info.duration, ccp(moveBypos.x/2,moveBypos.y/2)));//CC_CONTENT_SCALE_FACTOR()
//    }
}

void MYtextfieldttf::keyboardWillHide(CCIMEKeyboardNotificationInfo& info)
{
//    if (getParent() != NULL) {
//        CCPoint moveBypos = ccpAdd(ccp(info.end.origin.x - info.begin.origin.x,info.end.origin.y - info.begin.origin.y), ccp(info.end.size.width - info.begin.size.width,info.end.size.height - info.begin.size.height));
//        getParent()->runAction(CCMoveBy::create(info.duration, ccp(moveBypos.x/2,moveBypos.y/2)));
//    }
}

void MYtextfieldttf::keyboardDidHide(CCIMEKeyboardNotificationInfo& info)
{
    CCLOG("keyboardDidHide");
    SGNotificationCenter::sharedNotificationCenter()->postNotification(TEXTFIELDHIDE,NULL,false);
    //CCIMEDispatcher::sharedDispatcher()->removeDelegate(this);
    
}



