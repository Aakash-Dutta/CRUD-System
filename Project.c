#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
struct Menu
{
	int item_id;
	char item[25];
	float price;
};
int main()
{
	///necessary variables
	int u_choice;///1st choice for user
	int m_choice;///choice made in menu(add,update,delete)
	FILE *fp,*fp1;///file pointer to Menu.dat which stores the menu
	struct Menu m,*mptr;///structure variable and its pointer for the Menu data type
	mptr=&m;
	int m_size,i,m_change;///size of menu, loop counter variable , variable for item update in menu
	int m_delete;///item id to be stored to delete in menu
	char rec_choice;///recurring choice to add N number of data as user pleases.
	int j;///Loop counter variable
	char bitemname[50];///search-item for bill to menu
	int quantity1;///quantity of item purchased for bill

	///main code
	while(1)
	{
		system("cls");
		printf("%20s<<<<<< RESTAURANT MANAGEMENT SYSTEM >>>>>>\n"," ");
		printf("\n1.MENU");
		printf("\n2.Prepare Bill");
		printf("\n3.Exit");

		printf("\n\nYour Choice:\t");
		scanf("%d",&u_choice);

		/**Always open Menu.dat in append mode first to create a file so we don't face any errors*/
        	fp=fopen("Menu.dat","ab+");
        	fclose(fp);

        	///open file in rb+ mode to be able to update the datas easily
		fp=fopen("Menu.dat","rb+");


				///if file cant be open it returns null so exit the application
				if(fp==NULL)
				{
					printf("Cannot open file..\n\n");
					exit(-1);
				}


		///cases application
		switch (u_choice)
		{
			case 1:///MENU
				system("cls");
				printf("%15s<<<<<< MENU >>>>>>\n\n"," ");
				/**First Find weather the file is empty or not by applying fseek and ftell functions
				 * If the menu.dat is empty add the items
				 * If not view the items and give choice to either Add, Update or Delete the items
				 */
				fseek(fp,0,SEEK_END);
				m_size=ftell(fp)/sizeof(struct Menu);

				if(m_size==0)
				{
					printf("\nMenu is empty.Please add Items.\n\n");
					goto add_items;
				}

				///Display the MENU on the screen if file is not empty
				printf("%*s%*s%*s\n",-20,"Item_Id",-20,"Items",-20,"Price");
				rewind(fp);
				for(i=0;i<m_size;i++)
				{
					fread(&m,sizeof(struct Menu),1,fp);
					printf("%-20d%-20s%-20.2f\n",m.item_id,m.item,m.price);
				}

				///MENU add,update,delete choices
				printf("\n1.Add Items\t\t2.Update Item\t\t3.Delete Item.\t\t4.Back\n");
				printf("Input:\t");
				getchar();///clear input buffer
				scanf("%d",&m_choice);

				/**2nd switch case for sub menu,
				 * Case 1 should add items
				 * case 2 should update items
				 * case 3 should delete items
				 * user data must be continued until user press Y
				 */
				switch (m_choice)
				{
					case 1:///Add
						add_items:
						rec_choice='Y';
						while(rec_choice=='Y'||rec_choice=='y')
						{
							printf("\nEnter the item id:");
							scanf("%d",&(m.item_id));
							getchar();
							printf("Enter item name:");
							gets(m.item);
							printf("Enter price:");
							scanf("%f",&(m.price));

							fseek(fp,0,SEEK_END);

							///write data in file as user input is given
							if(fwrite(&m,sizeof(struct Menu),1,fp)==1)
								printf("Data Read..");
							else
								printf("Cant read.\n");

							printf("\nEnter another data? (Y/N)\t");
							getchar();
							scanf("%c",&rec_choice);
						}
						printf("\nPress any key..");
						break;
					case 2:///Update
						printf("\nEnter the item id of data you would like to change:");
						scanf("%d",&m_change);

						m_size=ftell(fp)/sizeof(struct Menu);
						rewind(fp);

						for(i=0;i<m_size;i++)
						{
							fread(&m,sizeof(struct Menu),1,fp);

							if(m_change==m.item_id)
							{
								printf("\nEnter the new item id:");
								scanf("%d",&(m.item_id));
								getchar();
								printf("Enter the new item name:");
								gets(m.item);
								printf("Enter the new price:");
								scanf("%f",&(m.price));

								fseek(fp,-sizeof(m),SEEK_CUR);
								if(fwrite(&m,sizeof(struct Menu),1,fp)==1)
									printf("\nData updated:");
								///taking the file pointer to the required position as it was changed
								fseek(fp,sizeof(m),SEEK_CUR);
							}
						}
						printf("\nPress any key..");
						break;
					case 3:///Delete
						/**To perform delete operation we create a file illusion
						 * First we find the item_id of data to be deleted
						 * Compare the item_id with data to be deleted
						 * After than copy all the remaining info to another Temporary file
						 * Then remove original file and rename the temporary file as the original file
						 */
						printf("\nEnter the item id of data you would like to delete:");
						scanf("%d",&m_delete);

						fseek(fp,0,SEEK_END);
						m_size=ftell(fp)/sizeof(struct Menu);
						rewind(fp);

						fp1=fopen("Temp.dat","wb");

						for(i=0;i<m_size;i++)
						{
							fread(&m,sizeof(struct Menu),1,fp);

							if(m_delete != m.item_id)
							{
								fwrite(&m,sizeof(struct Menu),1,fp1);
							}
							else
								printf("\nItem deleted..\n");
						}
						fclose(fp);
						fclose(fp1);
						remove("Menu.dat");
						rename("Temp.dat","Menu.dat");
						printf("\nPress any key..\n");
						break;
                    case 4:
                        printf("\nPress any key to confirm..");
                        break;
					default :
						printf("\n");
				}
				break;
			case 2:///Prepare Bill
			    /**To prepare the bill, we take the input of the item name and the quantity
			     *Compare the item name with data from the Menu.dat file using strcmp() function
			     *Then calculate the total price to be paid by asking for bill item until user needs.
			     */
                system("cls");
                printf("%20s<<<<<< BILLING SYSTEM >>>>>>\n\n"," ");

			    rec_choice='Y';
			    float total=0;

                while(rec_choice=='Y'||rec_choice=='y')
                {
                    getchar();
                    printf("\nItem name:");
                    gets(bitemname);
                    printf("Quantity:");
                    scanf("%d",&quantity1);

                    fseek(fp,0,SEEK_END);
                    m_size=ftell(fp)/sizeof(struct Menu);
                    rewind(fp);

                    for(j=0;j<m_size;j++)
                    {
                        fread(&m,sizeof(struct Menu),1,fp);

                        if(strcmp(bitemname,m.item)==0)
                        {
                            total=total+(m.price*quantity1);
                        }
                    }

                    printf("\nAdd another item?(Y/N)\t");
                    getchar();
                    scanf("%c",&rec_choice);
                 }
                printf("\nTotal price= %.2f\n",total);
                printf("\nPress any key..");
				break;
			case 3:
				exit(0);
			default:
				printf("\nWrong Choice.");
		}
		fclose(fp);
		getch();
	}

	return 0;
}




