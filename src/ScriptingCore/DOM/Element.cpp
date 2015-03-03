/**********************************************************\
Original Author: Richard Bateman (taxilian)

Created:    Dec 9, 2009
License:    Dual license model; choose one of two:
New BSD License
http://www.opensource.org/licenses/bsd-license.php
- or -
GNU Lesser General Public License, version 2.1
http://www.gnu.org/licenses/lgpl-2.1.html

Copyright 2009 PacketPass, Inc and the Firebreath development team
\**********************************************************/

#include "Deferred.h"
#include "Element.h"

using namespace FB::DOM;
using std::vector;

Element::Element(const FB::JSObjectPtr& element) : Node(element) {
}

Element::~Element() {
}

FB::DeferredPtr<std::string> Element::getInnerHTML() const {
    return getProperty<std::string>("innerHTML");
}
void Element::setInnerHTML(std::string html) const {
    setProperty("innerHTML", html);
}

FB::DeferredPtr<int> Element::getWidth() const {
    return getProperty<int>("width");
}
void Element::setWidth(const int width) const {
    setProperty("width", width);
}

FB::DeferredPtr<int> Element::getScrollWidth() const {
    return getProperty<int>("scrollWidth");
}

FB::DeferredPtr<int> Element::getHeight() const {
    return getProperty<int>("height");
}
void Element::setHeight(const int height) const {
    setProperty("height", height);
}

FB::DeferredPtr<int> Element::getScrollHeight() const {
    return getProperty<int>("scrollHeight");
}

FB::DeferredPtr<int> Element::getChildNodeCount() const {
    return getNode("childNodes")->thenPipe<int>([=](NodePtr ptr) {
        return ptr->getProperty<int>("length");
    });
}

FB::DeferredPtr<ElementPtr> Element::getChildNode(const int idx) const {
    return getElement("childNodes")->thenPipe<ElementPtr>([=](ElementPtr el) {
        return el->getElement(idx);
    });
}

FB::DeferredPtr<ElementPtr> Element::getParentNode() const {
    return getElement("parentNode")->convert_cast<ElementPtr>();
}

FB::DeferredPtr<ElementPtr> Element::getElementById(std::string id) const {
    return callMethod<JSObjectPtr>("getElementById", VariantList{ id })->then<ElementPtr>(
        [id](JSObjectPtr api) {
        return Element::create(api);
    });
}

FB::DeferredPtr<std::vector<ElementPtr>> Element::getElementsByTagName(const std::wstring& tagName) const {
    return getElementsByTagName(FB::wstring_to_utf8(tagName));
}

FB::DeferredPtr<std::vector<ElementPtr>> Element::getElementsByTagName(std::string tagName) const {
    return callMethod<vector<JSObjectPtr>>("getElementsByTagName", VariantList{ tagName })
        ->then<vector<ElementPtr>>([=](vector<JSObjectPtr> tagList) {
        std::vector<ElementPtr> outList;
        for (auto tag : tagList) {
            outList.emplace_back(Element::create(tag));
        }
        return outList;
    });
}

FB::DeferredPtr<std::string> FB::DOM::Element::getStringAttribute(std::string attr) const {
    return callMethod<std::string>("getAttribute", FB::VariantList{ attr });
}

