//==================================================================================//
//                                                                                  //
//  Copyright (c) 2019 Hugo Kindel <kindelhugo.pro@gmail.com>                       //
//                                                                                  //
//  This file is part of the Aryiele project.                                       //
//  Licensed under MIT License:                                                     //
//                                                                                  //
//  Permission is hereby granted, free of charge, to any person obtaining           //
//  a copy of this software and associated documentation files (the "Software"),    //
//  to deal in the Software without restriction, including without limitation       //
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,        //
//  and/or sell copies of the Software, and to permit persons to whom the           //
//  Software is furnished to do so, subject to the following conditions:            //
//                                                                                  //
//  The above copyright notice and this permission notice shall be included in      //
//  all copies or substantial portions of the Software.                             //
//                                                                                  //
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      //
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        //
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     //
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          //
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   //
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   //
//  SOFTWARE.                                                                       //
//                                                                                  //
//==================================================================================//

#include <Aryiele/Lexer/Lexer.h>
#include <Aryiele/Lexer/LexerTable.h>

namespace Aryiele {
    std::vector<LexerToken> Lexer::lex(const std::string& filepath) {
        std::ifstream file;
        std::string expression;

        file.open(filepath.c_str());

        if (file.fail())
            return m_tokens;

        while (getline(file, expression)) {
            stateMachine(expression);
            m_tokens.emplace_back(LexerToken("", LexerToken_Newline));
        }

        stateComments();
        stateStrings();
        stateNumbers();
        stateSpaces();

        return m_tokens;
    }
    
    std::string Lexer::getTokenName(const LexerToken& tokenType) {
        switch (tokenType.type) {
            case LexerToken_Number:
                return "Number";
            case LexerToken_String:
                return "String";
            case LexerToken_Character:
                return "Character";
            case LexerToken_Operator:
                return "Operator";
            case LexerToken_Identifier:
                return "Identifier";
            case LexerToken_Separator:
                return "Separator";
            case LexerToken_Space:
                return "Space";
            case LexerToken_Newline:
                return "Newline";
            case LexerToken_StringQuote:
                return "StringQuote";
            case LexerToken_Unknown:
                return "Unknown";
            default:
                return "Error";
        }
    }

    void Lexer::stateMachine(std::string expression) {
        LexerToken currentToken;
        std::vector<LexerToken> tokens;
        auto currentTransitionState = LexerToken_Reject;
        auto previousTransitionState = LexerToken_Reject;
        std::string currentTokenExpression;

        for (unsigned x = 0; x < expression.length();) {
            const auto currentCharacter = expression[x];
            const auto column = getTransitionTableColumn(currentCharacter);

            currentTransitionState = static_cast<LexerTokenEnum>(LexerTable[currentTransitionState][column]);

            if (currentTransitionState == LexerToken_Reject) {
                currentToken.content = currentTokenExpression;
                currentToken.type = previousTransitionState;
                currentTokenExpression = "";

                tokens.push_back(currentToken);
            }
            else {
                currentTokenExpression += currentCharacter;
                ++x;
            }

            previousTransitionState = currentTransitionState;
        }

        if (!currentTokenExpression.empty()) {
            currentToken.content = currentTokenExpression;
            currentToken.type = currentTransitionState;

            tokens.push_back(currentToken);
        }
        
        m_tokens.insert(m_tokens.end(), tokens.begin(), tokens.end());
    }

    void Lexer::stateComments() {
        LexerToken currentToken;
        std::vector<LexerToken> tokens;
        auto isInCommentSingleLine = false;
        auto isInCommentMultiLine = false;
        auto lengthInMultiline = 2;

        for (auto& token : m_tokens) {
            const auto lastToken = currentToken;
            currentToken = token;

            if (isInCommentSingleLine) {
                if (currentToken.type == LexerToken_Newline) {
                    tokens.emplace_back(currentToken);
                    isInCommentSingleLine = false;
                }
            } else if (isInCommentMultiLine) {
                if (currentToken.type == LexerToken_Newline) {
                    for (int i = 0; i < lengthInMultiline; i++) {
                        tokens.emplace_back(LexerToken("", LexerToken_Space));
                    }
                    
                    lengthInMultiline = 0;
                    tokens.emplace_back(currentToken);
                } else if (currentToken.content == "*/") {
                    lengthInMultiline += 2;
                    
                    for (int i = 0; i < lengthInMultiline; i++) {
                        tokens.emplace_back(LexerToken("", LexerToken_Space));
                    }
                    
                    isInCommentMultiLine = false;
                    lengthInMultiline = 0;
                } else {
                    lengthInMultiline += currentToken.content.size();
                }
            } else if (currentToken.content == "//") {
                isInCommentSingleLine = true;
            } else if (currentToken.content == "/*") {
                lengthInMultiline = 2;
                isInCommentMultiLine = true;
            } else {
                tokens.emplace_back(currentToken);
            }
        }

        m_tokens.clear();

        m_tokens = tokens;
    }

    void Lexer::stateStrings() {
        LexerToken currentToken;
        std::vector<LexerToken> tokens;
        std::string inTextQuote;
        std::string currentText;
        auto isInText = false;

        for (auto& token : m_tokens) {
            const auto lastToken = currentToken;
            currentToken = token;
            
            if (isInText) {
                if (currentToken.type == LexerToken_StringQuote && currentToken.content == inTextQuote) {
                    if (currentText[currentText.length() - 1] == '\\')
                    {
                        currentText.pop_back();
                        currentText += inTextQuote;
                    } else {
                        currentToken.content = currentText;
                        currentToken.type = currentText.size() == 1 ? LexerToken_Character : LexerToken_String;

                        tokens.emplace_back(currentToken);

                        isInText = false;
                        currentText = "";
                        inTextQuote = "";
                    }
                } else {
                    currentText += currentToken.content;
                }
            } else if (currentToken.type == LexerToken_StringQuote) {
                inTextQuote = currentToken.content;
                isInText = true;
            } else {
                tokens.emplace_back(currentToken);
            }
        }

        m_tokens.clear();

        m_tokens = tokens;
    }
    
    void Lexer::stateNumbers() {
        LexerToken currentToken;
        std::vector<LexerToken> tokens;
        std::string number;
        auto isInNumber = false;
        
        for (auto& token : m_tokens) {
            const auto lastToken = currentToken;
            currentToken = token;
    
            if (isInNumber) {
                if ((currentToken.type == LexerToken_Separator && currentToken.content == ".") ||
                    (currentToken.type == LexerToken_Number)) {
                    number += currentToken.content;
                } else {
                    tokens.emplace_back(LexerToken(number, LexerToken_Number));
                    tokens.emplace_back(currentToken);
    
                    isInNumber = false;
                    number = "";
                }
            } else if (currentToken.type == LexerToken_Number) {
                isInNumber = true;
                number = currentToken.content;
            } else {
                tokens.emplace_back(currentToken);
            }
        }
        
        m_tokens.clear();
        
        m_tokens = tokens;
    }
    
    void Lexer::stateSpaces() {
        LexerToken currentToken;
        std::vector<LexerToken> tokens;
    
        for (auto& token : m_tokens) {
            const auto lastToken = currentToken;
            currentToken = token;
            
            if (currentToken.type == LexerToken_Space) {
                tokens.emplace_back(LexerToken("", LexerToken_Space));
            } else {
                tokens.emplace_back(currentToken);
            }
        }
    
        m_tokens.clear();
    
        m_tokens = tokens;
    }

    LexerTokenEnum Lexer::getTransitionTableColumn(char currentCharacter) {
        if (currentCharacter == '{' || currentCharacter == '}' ||
            currentCharacter == '[' || currentCharacter == ']' ||
            currentCharacter == '(' || currentCharacter == ')' ||
            currentCharacter == ';' || currentCharacter == ',' ||
            currentCharacter == ':' || currentCharacter == '.') {
            return LexerToken_Separator;
        } else if (currentCharacter == '\"' || currentCharacter == '\'') {
            return LexerToken_StringQuote;
        } else if (isspace(currentCharacter)) {
                return LexerToken_Space;
        } else if (isdigit(currentCharacter)) {
            return LexerToken_Number;
        } else if (isalpha(currentCharacter) || currentCharacter == '_') {
            return LexerToken_Identifier;
        } else if (ispunct(currentCharacter)) {
            return LexerToken_Operator;
        }

        return LexerToken_Unknown;
    }

    Lexer &getLexer() {
        return Lexer::getInstance();
    }

} /* Namespace Aryiele. */
