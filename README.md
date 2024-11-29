# Minishell

REMEMBER TO SWITCH BRANCHES BEFORE WORKING ON ANY FEATURE

Ideal workflow:
+ main remains untouched
+ from main, create a develop(n+1)
+ each person pulls from develop(n+1) into a personal branch
+ work is done individually until a small feature is completed
+ merge request into develop(n+1) is checked by the other person
+ repeat until develop(n+1) reaches the agreed milestone, then merge with main
+ repeat process

Naming conv, if agreed: name every function starting with ms_ (example: ms_input_check.c).

Pull request conv, i agreed: we can check the checklists of the project sections while doing them before the pull request, and leave the general checkbox (the one numbered, often with x.x.x.x format) to be checked after branch test and merging.

Optional to-do: test dependency implementations
	-.d files?
	-Not die
