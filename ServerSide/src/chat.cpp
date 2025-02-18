#include "chat.hpp"




chat::chat(int lowerX, int upperX,int lowerY,int upperY) 
: lowerX(lowerX), upperX(upperX), lowerY(lowerY), upperY(upperY)
    
{
    maxCharsInOneLine = (upperX-lowerX)/13; // 10 is the average char width.?
    chatFont.loadFromFile("fonts/ARIAL.TTF");
    chatBar.setFont(chatFont);
    chatBar.setColor(sf::Color::Green);
    chatBar.setCharacterSize(charSize);
    chatBar.setPosition(lowerX+10, upperY+10);
    chatBackground.setSize(sf::Vector2f(upperX-lowerX, upperY-lowerY));
    chatBackground.setFillColor(sf::Color::Blue);
    chatBackground.setPosition(lowerX, lowerY);
}

void chat::addMessage(std::string message){
    std::stringstream ss;
    ss << message;
    sf::Text text;
    text.setFont(chatFont);
    text.setColor(sf::Color::Black);
    text.setCharacterSize(charSize);
    text.setPosition(lowerX+10, upperY-30);
    std::string line = "";
    std::string substr;
    while(ss >> substr){
        if(substr.size() > maxCharsInOneLine){
            while(substr.size() > maxCharsInOneLine){
                std::string sub = substr.substr(0, maxCharsInOneLine-line.size());
                substr = substr.substr(maxCharsInOneLine-line.size(), substr.size());
                text.setString(line + sub);
                shiftMessages();
                messages.insert(messages.begin(), text);
                line = "";
            }
            line.append(substr + " ");
            continue;
        }
        if (line.size() + substr.size() < maxCharsInOneLine){
            line.append(substr + " ");
        }else {
            text.setString(line);
            shiftMessages();
            messages.insert(messages.begin(), text);
            line = substr + " ";
        }
    }
    text.setString(line);
    shiftMessages();
    messages.insert(messages.begin(), text);
}


void chat::shiftMessages(){
    for(int i = 0; i < messages.size(); i++){
        messages[i].move(0, -20);
        if(messages[i].getPosition().y < lowerY){ // if text get past lower y limit deleted texts.
            messages.erase(messages.begin() + i, messages.end());
            break;
        }
    }
}

void chat::chatRender(sf::RenderWindow &window) {
    window.draw(chatBackground);
    for(int i = 0; i < messages.size(); i++){
        window.draw(messages[i]);
    }
    std::string chatBarText = chatBar.getString().toAnsiString();
    std::stringstream ss;
    ss << chatBarText;
    std::string line = "";
    std::string substr = "";
    sf::Text text;
    text.setFont(chatFont);
    text.setColor(sf::Color::Black);
    text.setCharacterSize(charSize);
    text.setPosition(lowerX+10, upperY+10);
    while (ss >> substr){
        if(substr.size() > maxCharsInOneLine){
            while(substr.size() > maxCharsInOneLine){
                std::string sub = substr.substr(0, maxCharsInOneLine-line.size());
                substr = substr.substr(maxCharsInOneLine-line.size(), substr.size());
                text.setString(line + sub);
                window.draw(text);
                line = "";
                text.move(0, 20);
            }
            line.append(substr + " ");
            continue;
        }
        if(line.size() + substr.size() > maxCharsInOneLine){
            text.setString(line);
            window.draw(text);
            line.clear();
            line.append(substr + " ");
            text.move(0, 20);
        } else {
            line.append(substr + " ");
        }
    }
    text.setString(line);
    window.draw(text);
    
}




void chat::chatBarInput(sf::Event event, Server &server) {
    // Handle backspace
    if(event.type == sf::Event::TextEntered){
        if (event.text.unicode >= 32 && event.text.unicode <= 126) {
            chatBar.setString(chatBar.getString() + static_cast<char>(event.text.unicode));
        } else {
            warning("Invalid character entered");
        }
        return;
    }
    if(event.type == sf::Event::KeyPressed){
        if(event.key.code == sf::Keyboard::BackSpace){
            std::string str = chatBar.getString().toAnsiString();
            if(str.size() > 0){
                str.pop_back();
            }
            chatBar.setString(str);
            return;
        }
        if(event.key.code == sf::Keyboard::Return){
                addMessage("ServerMessage: "+chatBar.getString()); // handle the input.
                sf::Packet packet;
                packet << chatBar.getString().toAnsiString();
                server.broadcastPacket(packet, "ServerMessage"); // broadcast the message to all clients.
                chatBar.setString("");
                return;
        }
    }
}
