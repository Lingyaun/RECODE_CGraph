#include "GElement.h"

std::string GElement::getName() const {
    return this->name_;
}

GElement::GElement() {

}

GElement::~GElement() {

};

GElement::GElement(const GElement& element) {
    this->done_ = element.done_;
    this->is_init_ = element.is_init_;
    this->run_before_ = element.run_before_;
    this->dependence_ = element.dependence_;
    this->left_depend_.store(element.left_depend_);
    this->linkable_ = element.linkable_;
    this->loop_ = element.loop_;
    this->name_ = element.name_;
}

GElement& GElement::operator=(const GElement& element) {
    if (this == &element) {
        return *this;
    }

    this->done_ = element.done_;
    this->is_init_ = element.is_init_;
    this->run_before_ = element.run_before_;
    this->dependence_ = element.dependence_;
    this->left_depend_.store(element.left_depend_);
    this->linkable_ = element.linkable_;
    this->loop_ = element.loop_;
    this->name_ = element.name_;

    return *this;
}

/**
 * 当前节点，所有依赖均执行完毕，切未被执行的时候，则可以执行
 * @return
 */
bool GElement::isRunnable() const {
    return 0 >= this->left_depend_ && false == this->done_;
}

bool GElement::isLinkable() const {
    return this->linkable_;
}

void GElement::setLoop(int loop)  {
    this->loop_ = (loop>=0)? loop:0; //循环次数不小于0
}

CSTATUS GElement::process(bool isMock) {
    CGRAPH_PROCESS_ERROR
}
