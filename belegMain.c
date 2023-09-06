// Nicolas Aßmann
// s85475
// 22/041/61

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"beleg.h"
#include"list.h"
#include<gtk/gtk.h>


char buf[128];
const char* search;

void on_entrysearch_changed(GtkEntry *e);


tList *pList;
tList * searchList;


GtkBuilder	*builder;


GtkWidget	*window;

// main buttons

GtkWidget	*buttonquit;
GtkWidget	*buttoninsert;
GtkWidget	*buttonsave;
GtkWidget	*buttonclear;

// sort buttons

GtkWidget	*buttonsortlname;
GtkWidget	*buttonsortfname;
GtkWidget	*buttonsorttel;

// insert widgets

GtkWidget	*entrylname;
GtkWidget	*entryfname;
GtkWidget	*entrytel;

GtkWidget	*buttoninserterrorok;

GtkWindow	*windowinserterror;
GtkWidget	*inserterrorwidget;

// search widgets 

GtkEntry	*entrysearch;
GtkWidget	*buttonsearchclear;


// output widgets

GtkTextBuffer 	*textbuffertel;
GtkTextBuffer 	*textbufferfname;
GtkTextBuffer 	*textbufferlname;

GtkWidget 	*textviewtel;
GtkWidget 	*textviewfname;
GtkWidget 	*textviewlname;

GtkTextIter	iter;

// delete widgets
GtkWindow	*deletewindow;
GtkWidget	*deletewidget;

GtkWidget	*buttondelete;
GtkWidget	*buttondeleteok;
GtkWidget	*buttondeletecancel;


// window insert error
void on_buttoninserterrorok_clicked()
{
	gtk_widget_hide(inserterrorwidget);
}

void inserterror()
{
	gtk_widget_get_visible(inserterrorwidget);
	gtk_widget_set_visible(inserterrorwidget,1);
	gtk_window_set_position(GTK_WINDOW(windowinserterror), (GTK_WIN_POS_CENTER_ALWAYS));
}



//outputfunktionen 

void gtkdispData(tDat *pDat)
{
	gtk_text_buffer_get_end_iter(textbufferlname, &iter);
	gtk_text_buffer_insert(textbufferlname, &iter,  pDat->lname,strlen(pDat->lname));
    
	gtk_text_buffer_get_end_iter(textbufferlname, &iter);
	gtk_text_buffer_insert(textbufferlname, &iter,  "\n",1);
	
	gtk_text_buffer_get_end_iter(textbufferfname, &iter);
	gtk_text_buffer_insert(textbufferfname, &iter,  pDat->fname,strlen(pDat->fname));
    
	gtk_text_buffer_get_end_iter(textbufferfname, &iter);
	gtk_text_buffer_insert(textbufferfname, &iter,  "\n",1);

	gtk_text_buffer_get_end_iter(textbuffertel, &iter);
	gtk_text_buffer_insert(textbuffertel, &iter,  pDat->tel,strlen(pDat->tel));
    	
	gtk_text_buffer_get_end_iter(textbuffertel, &iter);
	gtk_text_buffer_insert(textbuffertel, &iter,  "\n",1);
    
}


void on_buttonout_clicked()
{
	GtkTextIter 	start;
	gtk_text_buffer_get_start_iter(textbufferlname,&start);
	gtk_text_buffer_get_end_iter(textbufferlname,&iter);
	gtk_text_buffer_delete(textbufferlname,&start,&iter);

	gtk_text_buffer_get_start_iter(textbufferfname,&start);
	gtk_text_buffer_get_end_iter(textbufferfname,&iter);
	gtk_text_buffer_delete(textbufferfname,&start,&iter);
	
	gtk_text_buffer_get_start_iter(textbuffertel,&start);
	gtk_text_buffer_get_end_iter(textbuffertel,&iter);
	gtk_text_buffer_delete(textbuffertel,&start,&iter);
	
	
	for(tDat *pDat=getFirst(searchList);pDat;pDat=getNext(searchList)) gtkdispData(pDat);
}




// input funktionen





tDat * getData()
{
	tDat* pd=malloc(sizeof(tDat));
	if(pd)
	{
		sprintf(buf,gtk_entry_get_text(GTK_ENTRY(entrylname)));
		if(strlen(buf)<1 || buf[0]==' ') { printf("error no input in lastname!\n");inserterror(); return NULL;}
		pd->lname=malloc(sizeof(char) * strlen(buf));
		strcpy(pd->lname,buf);
		
		sprintf(buf,gtk_entry_get_text(GTK_ENTRY(entryfname)));
		if(strlen(buf)<1 || buf[0]==' ') { printf("error no input in fastname!\n");inserterror(); return NULL;}
		pd->fname=malloc(sizeof(char) * strlen(buf));
		strcpy(pd->fname,buf);
		
		sprintf(buf,gtk_entry_get_text(GTK_ENTRY(entrytel)));
		if(strlen(buf)<1 || buf[0]==' ') { printf("error no input in phonenumber!\n");inserterror(); return NULL;}
		pd->tel=malloc(sizeof(char) * strlen(buf));
		strcpy(pd->tel,buf);
	}
	else printf("error could not allocate memory!\n");

	return pd;
}

void on_buttonclear_clicked()
{
	gtk_editable_delete_text (GTK_EDITABLE(entrylname), 0, -1);
	gtk_editable_delete_text (GTK_EDITABLE(entryfname), 0, -1);
	gtk_editable_delete_text (GTK_EDITABLE(entrytel), 0, -1);

}

void on_buttoninsert_clicked()
{
	tDat *pDat=NULL;
	pDat=getData();
	if(pDat) 
	{
		int ret=insertTail(pList,pDat);
       		if(ret==0) printf("error, could not insert in list!\n");
		on_entrysearch_changed(entrysearch);
		on_buttonclear_clicked();
		on_buttonout_clicked();
	}


}




// compare functions
int cmpFname(const void *p1, const void *p2)
{
	return strcmp(((tDat*)p1)->fname,((tDat*)p2)->fname);	
}
int cmpLname(const void *p1, const void *p2)
{
	return strcmp(((tDat*)p1)->lname,((tDat*)p2)->lname);	
}
int cmpTel(const void *p1, const void *p2)
{
	return strcmp(((tDat*)p1)->tel,((tDat*)p2)->tel);	
}

// search

void on_entrysearch_changed(GtkEntry *e)
{
		
	for(tDat *pDat=getFirst(searchList);pDat;pDat=getFirst(searchList)) removeFirst(searchList);
	search = gtk_entry_get_text(e);
	
	for(tDat *pDat=getFirst(pList);pDat;pDat=getNext(pList)) 
		{
			if(strncmp(pDat->fname,search,strlen(search))==0 || strncmp(pDat->lname,search,strlen(search))==0 || strncmp(pDat->tel,search,strlen(search))==0 ) 
			{
				
				int ret=insertTail(searchList,pDat);
                if(ret==0) printf("error, could not insert in list!\n");
			}
		}
	
	on_buttonout_clicked();


}

void on_buttonsearchclear_clicked()
{
	gtk_editable_delete_text (GTK_EDITABLE(entrysearch), 0, -1);

}
// delete functions

void on_buttondeletecancel_clicked()
{
	gtk_widget_hide(deletewidget);
}


void freeDat(tDat *pDat)
{
	free(pDat->lname);
	free(pDat->fname);
	free(pDat->tel);
	free(pDat);
}


void on_buttondeleteok_clicked()
{
	for(tDat *pDatS=getFirst(searchList);pDatS;pDatS=getNext(searchList)) 
	{
		for(tDat *pDat=getFirst(pList);pDat;pDat=getNext(pList)) 
		{
			if(pDatS==pDat) 
			{
				
				freeDat(pDat);
				pDatS=getPrevious(searchList);
				pDat=getPrevious(pList);
				removeNext(searchList);
				removeNext(pList);
				break;
			}

		}
	}
	gtk_widget_hide(deletewidget);
	on_buttonout_clicked();
}

void on_buttondelete_clicked()
{
	gtk_widget_get_visible(deletewidget);
	gtk_widget_set_visible(deletewidget,1);
	gtk_window_set_position(GTK_WINDOW(deletewindow), (GTK_WIN_POS_CENTER_ALWAYS));

}

void on_buttonsortlname_clicked()
{
	quicksort(searchList,cmpLname);
	on_buttonout_clicked();	
}


void on_buttonsortfname_clicked()
{	
	mergesort(searchList,cmpFname);
	on_buttonout_clicked();	
}

void on_buttonsorttel_clicked()
{	
	quicksort(searchList,cmpTel);
	on_buttonout_clicked();	
}


void on_buttonsearch_clicked()
{

}

void on_buttonsave_clicked()
{
	writeDat(pList);	
}

void on_buttonquit_clicked()
{
	while(getFirst(searchList)) removeFirst(searchList); 
	tDat *temp=getFirst(pList);
	while(temp)
	{
		freeDat(temp);
		removeFirst(pList);
		temp=getFirst(pList);
	}

	free(pList);
	free(searchList);
	gtk_main_quit();
}


int main(int argc, char *argv[])
{
	

	printf("Telefonverzeichniss\n");
	pList=CreateList();
	searchList=CreateList();

	readDat(pList);

	for(tDat *pDat=getFirst(pList);pDat;pDat=getNext(pList)) 
    {
        int ret=insertTail(searchList,pDat); 
        if(ret==0) printf("error, could not insert in list!\n");
    }
// initialise gtk ,		 assign  builder,window,textview,entry,textbuffer 
	gtk_init(&argc, &argv);

	builder = gtk_builder_new_from_file ("beleg.glade");
	
	window = GTK_WIDGET(gtk_builder_get_object(builder,"window"));

	textviewtel = GTK_WIDGET(gtk_builder_get_object(builder,"textviewtel"));
	textviewfname = GTK_WIDGET(gtk_builder_get_object(builder,"textviewfname"));
	textviewlname = GTK_WIDGET(gtk_builder_get_object(builder,"textviewlname"));

// insert entrys
	entrytel = GTK_WIDGET(gtk_builder_get_object(builder,"entrytel"));
	entryfname = GTK_WIDGET(gtk_builder_get_object(builder,"entryfname"));
	entrylname = GTK_WIDGET(gtk_builder_get_object(builder,"entrylname"));

	
	windowinserterror = GTK_WINDOW(gtk_builder_get_object(builder,"windowinserterror"));
	inserterrorwidget = GTK_WIDGET(gtk_builder_get_object(builder,"windowinserterror"));
	g_signal_connect(inserterrorwidget, "delete_event", G_CALLBACK(on_buttoninserterrorok_clicked), NULL);

	buttoninserterrorok = GTK_WIDGET(gtk_builder_get_object(builder,"buttoninserterrorok"));
	g_signal_connect(buttoninserterrorok, "clicked", G_CALLBACK(on_buttoninserterrorok_clicked), NULL);

// delete Widgets

	deletewidget = GTK_WIDGET(gtk_builder_get_object(builder,"deletewindow"));

	deletewindow = GTK_WINDOW(gtk_builder_get_object(builder,"deletewindow"));

	buttondelete = GTK_WIDGET(gtk_builder_get_object(builder,"buttondelete"));
	buttondeleteok = GTK_WIDGET(gtk_builder_get_object(builder,"buttondeleteok"));
	buttondeletecancel = GTK_WIDGET(gtk_builder_get_object(builder,"buttondeletecancel"));

	g_signal_connect(buttondelete, "clicked", G_CALLBACK(on_buttondelete_clicked), NULL);
	g_signal_connect(buttondeleteok, "clicked", G_CALLBACK(on_buttondeleteok_clicked), NULL);
	g_signal_connect(buttondeletecancel, "clicked", G_CALLBACK(on_buttondeletecancel_clicked), NULL);
	g_signal_connect(deletewidget, "delete_event", G_CALLBACK(on_buttondeletecancel_clicked), NULL);

// search entrys 
	entrysearch = GTK_ENTRY(gtk_builder_get_object(builder,"entrysearch"));
    g_signal_connect (entrysearch, "changed", G_CALLBACK (on_entrysearch_changed), NULL);

	buttonsearchclear = GTK_WIDGET(gtk_builder_get_object(builder,"buttonsearchclear"));
	g_signal_connect(buttonsearchclear, "clicked", G_CALLBACK(on_buttonsearchclear_clicked), NULL);
	
	textbuffertel = gtk_text_view_get_buffer (GTK_TEXT_VIEW(textviewtel));
	textbufferfname = gtk_text_view_get_buffer (GTK_TEXT_VIEW(textviewfname));
	textbufferlname = gtk_text_view_get_buffer (GTK_TEXT_VIEW(textviewlname));
	
	g_signal_connect(window, "destroy", G_CALLBACK(on_buttonquit_clicked), NULL);

	
	
// buttons basic options  (out,insert,search,save,quit)

	buttoninsert = GTK_WIDGET(gtk_builder_get_object(builder,"buttoninsert"));
	g_signal_connect (buttoninsert, "clicked", G_CALLBACK (on_buttoninsert_clicked), NULL);
        
	
	buttonclear = GTK_WIDGET(gtk_builder_get_object(builder,"buttonclear"));
	g_signal_connect (buttonclear, "clicked", G_CALLBACK (on_buttonclear_clicked), NULL);
	
	buttonsave = GTK_WIDGET(gtk_builder_get_object(builder,"buttonsave"));
   	g_signal_connect (buttonsave, "clicked", G_CALLBACK (on_buttonsave_clicked), NULL);
		
	buttonquit = GTK_WIDGET(gtk_builder_get_object(builder,"buttonquit"));
	g_signal_connect (buttonquit, "clicked", G_CALLBACK (on_buttonquit_clicked), NULL);

// sortbuttons

	buttonsortlname = GTK_WIDGET(gtk_builder_get_object(builder,"buttonsortlname"));
	g_signal_connect (buttonsortlname, "clicked", G_CALLBACK (on_buttonsortlname_clicked), NULL);
	
	buttonsortfname = GTK_WIDGET(gtk_builder_get_object(builder,"buttonsortfname"));
	g_signal_connect (buttonsortfname, "clicked", G_CALLBACK (on_buttonsortfname_clicked), NULL);
	
	buttonsorttel = GTK_WIDGET(gtk_builder_get_object(builder,"buttonsorttel"));
	g_signal_connect (buttonsorttel, "clicked", G_CALLBACK (on_buttonsorttel_clicked), NULL);

	on_buttonout_clicked();
	
	
	gtk_main();

	return EXIT_SUCCESS;
}
