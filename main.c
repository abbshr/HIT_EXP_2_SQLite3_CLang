#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

typedef struct College
{
    char* cName;
    char* state;
    char* enrollment;
} College;

typedef struct Student
{
    char* sID;
    char* sName;
    char* GPA;
    char* sizeHS;
} Student;

typedef struct Apply
{
    char* sID;
    char* cName;
    char* major;
    char* decision;
} Apply;

static int view_all_callback(void* data, int argc, char** argv, char** azColName)
{
    int i;
    fprintf(stderr, "%s:\n", (const char*)data);
    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

static int insert_table_callback(void* NotUsed, int argc, char** argv, char** azColName)
{
    int i = 0;
    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    return 0;
}

static int create_table_callback(void* NotUsed, int argc, char** argv, char** azColName)
{
    return 0;
}

int main(int argc, char* argv[])
{
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    char* sql;
    const char* data = "Callback function called";
    /* 创建数据库连接 */
    rc = sqlite3_open("DBforCourse.db3", &db);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* 建表*/
    sql = "create table College(cName text, state text, enrollment int);";
    rc = sqlite3_exec(db, sql, create_table_callback, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        printf("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "College Table created successfully\n");
    }
    sql = "create table Student(sID int, sName text, GPA float, sizeHS int);";
    rc = sqlite3_exec(db, sql, create_table_callback, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        printf("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Student Table created successfully\n");
    }
    sql = "create table Apply(sID int, cName text, major text, decision text);";
    rc = sqlite3_exec(db, sql, create_table_callback, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        printf("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Apply Table created successfully\n");
    }

    /* 加载文件*/
    College college[4];
    Student student[12];
    Apply apply[19];
    FILE* fp;
    char line[30];
    char delims[] = ",";
    char* result = NULL;
    int i = 0;
    if(fp = fopen("dbcollege.txt", "r"))
    {
        while (fgets(line, 100, fp) != NULL)
        {
            result = strtok(line, delims);
            college[i].cName = result;
            result = strtok(NULL, delims);
            college[i].state = result;
            result = strtok(NULL, delims);
            college[i].enrollment = result;
            result = NULL;
            //printf("%s, %s, %s\n", college[i].cName, college[i].state, college[i].enrollment);
            i++;
        }
        fclose(fp);
    }
    i = 0;
    if(fp = fopen("dbstudent.txt", "r"))
    {
        while (fgets(line, 100, fp) != NULL)
        {
            result = strtok(line, delims);
            student[i].sID = result;
            result = strtok(NULL, delims);
            student[i].sName = result;
            result = strtok(NULL, delims);
            student[i].GPA = result;
            result = strtok(NULL, delims);
            student[i].sizeHS = result;
            result = NULL;
            //printf("%s, %s, %s, %s\n", student[i].sID, student[i].sName, student[i].GPA, student[i].sizeHS);
            i++;
        }
        fclose(fp);
    }
    i = 0;
    if(fp = fopen("dbapply.txt", "r"))
    {
        while (fgets(line, 100, fp) != NULL)
        {
            result = strtok(line, delims);
            apply[i].sID = result;
            result = strtok(NULL, delims);
            apply[i].cName = result;
            result = strtok(NULL, delims);
            apply[i].major = result;
            result = strtok(NULL, delims);
            apply[i].decision = result;
            result = NULL;
            //printf("%s, %s, %s, %s\n", apply[i].sID, apply[i].cName, apply[i].major, apply[i].decision);
            i++;
        }
        fclose(fp);
    }
    i = 0;

    /* 导入表*/
    while (i < 4) {
        sql = sqlite3_mprintf("insert into College(cName, state, enrollment) values('%s', '%s', '%d');", college[i].cName, college[i].state, atoi(college[i].enrollment));
        rc = sqlite3_exec(db, sql, insert_table_callback, 0, &zErrMsg);
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            fprintf(stdout, "operations done successfully\n");
        }
        i++;
        sqlite3_free(sql);
    }
    i = 0;
    while (i < 12) {
        sql = sqlite3_mprintf("insert into Student(sID, sName, GPA, sizeHS) values('%d', '%s', '%f', '%d');", atoi(student[i].sID), student[i].sName, atof(student[i].GPA), atoi(student[i].sizeHS));
        rc = sqlite3_exec(db, sql, insert_table_callback, 0, &zErrMsg);
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            fprintf(stdout, "operations done successfully\n");
        }
        i++;
        sqlite3_free(sql);
    }
    i = 0;
    while (i < 19) {
        sql = sqlite3_mprintf("insert into Apply(sID, cName, major, decision) values('%d', '%s', '%s', '%s');", atoi(apply[i].sID), apply[i].cName, apply[i].major, apply[i].decision);
        rc = sqlite3_exec(db, sql, insert_table_callback, 0, &zErrMsg);
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            fprintf(stdout, "operations done successfully\n");
        }
        i++;
        sqlite3_free(sql);
    }

    /* select in Student GPA > 3.6, return sID, sName and GPA */
    sql = "select sID, sName, GPA from Student where GPA > 3.6;";
    rc = sqlite3_exec(db, sql, view_all_callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "operations done successfully\n");
    }

    /* select in Apply and Student distinct, return sName, cName*/
    sql = "select distinct sName, cName from Student nature join Apply;";
    rc = sqlite3_exec(db, sql, view_all_callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "operations done successfully\n");
    }

    /* select*/
    sql = "select sName, GPA, decision from Student nature join Apply where sizeHS < 1000 and major = \"CS\" and cName = \"Stanford\";";
    rc = sqlite3_exec(db, sql, view_all_callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "operations done successfully\n");
    }

    /**/
    sql = "select * from Apply nature join Student order by GPA desc, enrollment;";
    rc = sqlite3_exec(db, sql, view_all_callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "operations done successfully\n");
    }

    /**/
    sql = "select sID, sName, GPA*(sizeHS / 1000) as newGPA from Student;";
    rc = sqlite3_exec(db, sql, view_all_callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "operations done successfully\n");
    }

    /**/
    sql = "select max(cName) from College;";
    rc = sqlite3_exec(db, sql, view_all_callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "operations done successfully\n");
    }

    /**/
    sql = "select * from College nature join Student nature join Apply;";
    rc = sqlite3_exec(db, sql, view_all_callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "operations done successfully\n");
    }

    /**/
    sql = "select cName from Apply where count(major) < 5 group by cName;";
    rc = sqlite3_exec(db, sql, view_all_callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "operations done successfully\n");
    }

    sqlite3_close(db);
    return 0;
}
