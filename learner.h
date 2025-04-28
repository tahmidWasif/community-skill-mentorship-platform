#ifndef LEARNER_H
#define LEARNER_H

void manage_issues(const char*);
void get_mentor_ip(char [], char []);
void mentor_chat();
void view_mentors();
int validate_learner(const char*, const char*);
void signup_learner();
void submit_issue(const char*);
void view_comments(const char*);
void learner_entry();
int mainLearner();

#endif