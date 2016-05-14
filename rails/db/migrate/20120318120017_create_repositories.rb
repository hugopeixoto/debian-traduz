class CreateRepositories < ActiveRecord::Migration
  def change
    create_table :repositories do |t|
      t.text :name
      t.text :path
      t.text :last_commit

      t.timestamps
    end
  end
end
