#ifndef MENTOR_H
#define MENTOR_H

void get_learner_ip(char [], char[]);
void learner_chat();
int validate_mentor(const char*, const char*, char*);
void signup_mentor();
void view_issues(const char*);
void add_comment(const char*, const char*);
void mentor_entry();
int mainMentor();

#endif