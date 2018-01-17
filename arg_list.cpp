#include "arg_list.hpp"

arg_list::arg_list() : length(0), first(NULL), current(NULL) {
	this->first = this->current = this->last = new arg_item;
	this->first->index = 0;
	this->first->value = NULL;
	this->first->next = NULL;
}

arg_list::~arg_list() {
	this->current = this->first;
	while( this->current != this->last ) {
		arg_item *temp = this->current->next;
		delete this->current;
		this->current = temp;
	}
	delete this->last;
}

void arg_list::add( php_var *arg ) {
	this->last->value = arg;
	this->last->next = new arg_item;
	this->last->next->index = this->last->index + 1;
	this->last = this->last->next;
	this->length++;
}

const php_var* arg_list::cur() {
	if( this->current != NULL )
		return this->current->value;
	else
		return NULL;
}

const php_var* arg_list::fetch() {
	php_var *temp = this->current->value;
	if( this->current != this->last ) {
		this->current = this->current->next;
	}
	return temp;
}

int arg_list::pos() {
	if( this->current != NULL )
		return this->current->index;
	else
		return -1;
}

int arg_list::len() {
	return this->length;
}

void arg_list::start() {
	this->current = this->first;
}

void arg_list::skip( int num ) {
	while( --num ) {
		if( this->current != NULL && this->current != this->last )
			this->current = this->current->next;
		else
			return;
	}
}

void arg_list::end() {
	while( this->current->next != this->last )
		this->skip();
}
