/*
 ============================================================================
 Aufgabe     : Datenstrukturen - Woche 4
 Autor       : Burst, Luca
 Matrikel    : 4581869
 Version     : 1.0.0.0
 ============================================================================
 */
#include <stdbool.h>
#include <stdlib.h>
#include "dhbwstudentlist.h"
#include "dhbwstudent.h"


//DIESE METHODEN NICHT AENDERN

StudentLP StudentLPAlloc(Student_p newStudent) {

	StudentLP new = malloc(sizeof(StudentL));

	new->student = newStudent;
	new->next = NULL;

	return new;
}

void StudentLInsertFirst(StudentLP *anchor_adr, Student_p newStudent) {
	StudentLP oldFirst = *anchor_adr;
	StudentLP newFirst = StudentLPAlloc(newStudent);
	*anchor_adr = newFirst;
	newFirst->next = oldFirst;
}

void StudentLInsertLast(StudentLP *anchor_adr, Student_p newStudent) {

	StudentLP current = *anchor_adr;
	StudentLP previous = NULL;

	StudentLP newLP = StudentLPAlloc(newStudent);

	while (current != NULL) {
		previous = current;
		current = current->next;
	}

	if (previous != NULL) {
		previous->next = newLP;
	} else {
		*anchor_adr = newLP;
	}

}

int StudentLSize(StudentLP *anchor_adr) {

	StudentLP current = *anchor_adr;

	int size = 0;

	while (current != NULL) {
		size++;
		current = current->next;
	}

	return size;
}

void StudentLPFree(StudentLP info) {
	if (info == NULL)
		return;

	StudentFree(info->student);
	free(info);
	return;
}

void StudentLFree(StudentLP *anchor_adr) {

	StudentLP current = *anchor_adr;

	while (current) {
		StudentLP newCurrent = current->next;
		StudentLPFree(current);
		current = newCurrent;
	}

	*anchor_adr = NULL;
	return;
}

StudentLP deepLPCopy(StudentLP info) {
	if (info == NULL)
		return NULL;
	StudentLP copy = StudentLPAlloc(deepCopy(info->student));
	copy->next = NULL;
	return copy;
}

StudentLP* deepLCopy(StudentLP *anchor_adr) {
	if (anchor_adr == NULL)
		return NULL;

	StudentLP *copy = malloc(sizeof(StudentLP));
	*copy = NULL;

	StudentLP current = *anchor_adr;

	while (current) {
		StudentLInsertLast(copy, deepCopy(current->student));
		current = current->next;
	}

	return copy;
}

StudentLP* StudentsFromFile(char *filename) {

	FILE *in = fopen(filename, "r");

	char string[BUF_SIZE];
	StudentLP *all_students = malloc(sizeof(StudentLP));
	*all_students = NULL;
	StudentLP reverse_students_anchor = NULL;
	StudentLP *reverse_students = &reverse_students_anchor;

	while (fgets(string, BUF_SIZE, in)) {

		//remove newline (works for both windows and unix)
		string[strcspn(string, "\r\n")] = 0;

		//printf("%s\n", string);
		StudentLInsertFirst(reverse_students, StudentAlloc(string));
	}

	//create normal ordered list
	{
		StudentLP current = *reverse_students;
		while (current != NULL) {
			StudentLInsertFirst(all_students, deepCopy(current->student));
			current = current->next;
		}
	}

	StudentLFree(reverse_students);

	return all_students;
}

//Bis hier nicht ?ndern



//Ab hier Aufgaben

bool StudentLImplemented() {
	//TODO: hier auf true aendern, damit Ihre Implementierung getestet wird
	return true;
}

bool StudentLContainsStudent(StudentLP *anchor_adr, Student_p student) {
	StudentLP akt = *anchor_adr;
	if (akt == NULL) return NULL;
	
	while (akt != NULL) {
		Student_p temp = akt->student;
		if (strcmp(temp->lastname, student->lastname) == 0 && temp->matrnr == student->matrnr)			return true;

		if (akt->next == NULL)	return NULL;
		else					akt = akt->next;
	}
	return false;
}

StudentLP StudentLExtractStudent(StudentLP *anchor_adr, Student_p student) {
	StudentLP akt = *anchor_adr, pre = NULL;

	//empty list check
	if (akt == NULL) return NULL;

	while (akt != NULL) {

		//Find the student
		if (strcmp(akt->student->lastname, student->lastname) == 0 && akt->student->matrnr == student->matrnr) {
			StudentLP temp = akt;

			if (pre != NULL)
			{
				pre->next = akt->next;
			}
			else if (akt->next)
			{
				akt = akt->next;
				*anchor_adr = akt;
			}
			else
			{
				*anchor_adr = NULL;
			}
			return temp;
		}
	

		if (akt->next == NULL)
		{
			//end of list
			return NULL;
		}
		else {
			//next element
			pre = akt;
			akt = akt->next;
		}
	}
}

StudentLP StudentLFindStudent(StudentLP *anchor_adr, Student_p student) {
	StudentLP akt = *anchor_adr;
	if (akt == NULL) return NULL;

	while (akt != NULL) {
		Student_p temp = akt->student;
		if (strcmp(temp->lastname, student->lastname) == 0 && temp->matrnr == student->matrnr)			return akt;

		if (akt->next == NULL)	return NULL;
		else					akt = akt->next;
	}
}

StudentLP StudentLFindByMatr(StudentLP *anchor_adr, int matrnr) {
	StudentLP akt = *anchor_adr;
	if (akt == NULL) return NULL;

	while (akt != 0) {
		Student_p temp = akt->student;
		if (temp->matrnr == matrnr)	return akt;
		
		if (akt->next == NULL)	return NULL;
		else					akt = akt->next;
	}
}

StudentLP StudentLFindByName(StudentLP *anchor_adr, char *lastname) {
	StudentLP akt = *anchor_adr;
	if (akt == NULL) return NULL;

	while (akt != 0) {
		Student_p temp = akt->student;
		if (strcmp(temp->lastname, lastname) == 0)	return akt;

		if (akt->next == NULL)	return NULL;
		else					akt = akt->next;
	}
}

void StudentLInsertSorted(StudentLP *anchor_adr, Student_p newStudent) {
	StudentLP akt = *anchor_adr, pre = NULL, newStudentLP = NULL;

	if (newStudent->matrnr == 0) {
		return;
	}

	if (*anchor_adr == NULL) {
		StudentLInsertFirst(anchor_adr, newStudent);
		return;
	}

	while (akt)
	{
		if (akt->next == NULL) // If current is tail
		{
			if (newStudent->matrnr < akt->student->matrnr)
			{
				newStudentLP = StudentLPAlloc(newStudent);

				if (pre == NULL)
				{
					// Set newStudent as new head of list
					StudentLP oldHead = *anchor_adr;
					*anchor_adr = newStudentLP;
					newStudentLP->next = oldHead;
				}
				else
				{
					// place in between
					pre->next = newStudentLP;
				}
			}
			else
			{
				if (StudentLContainsStudent(anchor_adr, newStudent))
				{
					//avoid redundancy
					return;
				}
				else
				{
					StudentLInsertLast(anchor_adr, newStudent);
				}
			}

			return;
		}
		else // not tail
		{
			newStudentLP = StudentLPAlloc(newStudent);

			if (akt->student->matrnr < newStudent->matrnr && akt->next->student->matrnr > newStudent->matrnr)
			{
				newStudentLP->next = akt->next;
				akt->next = newStudentLP;
				return;
			}

			if (newStudent->matrnr < akt->student->matrnr)
			{
				if (pre == NULL) // akt is head
				{
					StudentLInsertFirst(anchor_adr, newStudent);
				}
				else
				{
					pre->next = newStudentLP;
					newStudentLP->next = akt;
				}

				return;
			}
			else
			{
				// set before and get next
				pre = akt;
				akt = akt->next;
			}
		}
	}
}

